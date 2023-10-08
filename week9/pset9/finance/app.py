import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get user stock from database
    user_stocks = db.execute(
        "SELECT ep.symbol, SUM(ep.qty) AS shares FROM expenses AS ep JOIN users ON ep.user_id = users.id WHERE users.id = ? GROUP BY ep.symbol ORDER BY ep.symbol ASC", session["user_id"])

    # Get user cash from database
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    user_cash = float(user_cash[0]["cash"])

    # Get current quotes from API based on user info
    quotes = []
    for stock in user_stocks:
        result = lookup(stock["symbol"])
        quotes.append(result)

    # Calculate total cost based on user spent
    total_cost = 0
    for index, data in enumerate(quotes):
        if not data["price"]:
            continue
        total_cost = (float(data["price"]) * user_stocks[index]["shares"]) + total_cost

    return render_template("index.html", user_stocks=user_stocks, quotes=quotes, user_cash=user_cash, total_cost=total_cost, enumerate=enumerate, int=int)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        user_id = session["user_id"]

        # Get user cash from database
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

        # Ensure stock symbol was submitted
        if not symbol:
            return apology("must provide stock symbol", 400)

        # Get a stock quote by looking up the stock symbol
        result_quote = lookup(symbol)

        # Ensure valid stock symbol or found quote
        if result_quote == None or not result_quote:
            return apology("invalid stock symbol or not found quotes", 400)

        # Ensure share input is a positive integer
        try:
            input_shares = int(request.form.get("shares"))
        except (ValueError, TypeError):
            return apology("invalid input for number of shares", 400)

        if input_shares <= 0:
            return apology("invalid number of shares", 400)

        # Ensure user has enough cash to buy shares
        elif user_cash[0]["cash"] < (float(result_quote["price"]) * input_shares):
            return apology("you don't have enough cash in your account", 400)

        # Create transection
        db.execute("INSERT INTO expenses (user_id, symbol, qty, price, datetime) VALUES (?, ?, ?, ?, ?)",
                   user_id, result_quote["symbol"], input_shares, float(result_quote["price"]), datetime.datetime.now())

        # Update user cash
        updated_user_cash = user_cash[0]["cash"] - (float(result_quote["price"]) * input_shares)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_user_cash, user_id)

        # Redirect the user to the home page
        flash("Bought!")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    histories = db.execute(
        "SELECT symbol, qty, price, datetime FROM expenses JOIN users ON expenses.user_id = users.id WHERE users.id = ? ORDER BY datetime DESC", session["user_id"])

    return render_template("history.html", histories=histories)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":

        symbol = request.form.get("symbol")

        # Ensure stock symbol was submitted
        if not symbol:
            return apology("must provide stock symbol", 400)

        # Get a stock quote by looking up the stock symbol
        result_quote = lookup(symbol)

        # Ensure valid stock symbol or found quote
        if result_quote == None or not result_quote:
            return apology("invalid stock symbol or not found quotes", 400)

        # Redirect user to quote page with quote detail
        return render_template("quote.html", result_quote=result_quote)

    else:

        # set default data
        stock = {
            "name": "N/A",
            "price": 0.0,
            "symbol": "N/A"
        }

        # Redirect user to quote page with default data
        return render_template("quote.html", result_quote=stock)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        name = request.form.get("username")
        password = request.form.get("password")
        confirm_password = request.form.get("confirmation")

        # Query database for username to check if this username already exists
        rows = db.execute("SELECT * FROM users WHERE username = ?", name)
        if len(rows) == 1:
            return apology("the username already exists", 400)

        # Ensure username was submitted
        elif not name:
            return apology("username field is required", 400)

        # Ensure username and password and confirm password were submitted
        elif not password or not confirm_password:
            return apology("password field is required", 400)

        # Ensure password and confirm password must match
        elif password != confirm_password:
            return apology("the password and confirm password fields must match", 400)

        # Insert username and generate hash into users table
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", name, generate_password_hash(password))

        # Auto login after user successfully register

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", name)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect the user to the home page
        flash("Registered!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Get user stocks from database
    user_stocks = db.execute(
        "SELECT ep.symbol, SUM(ep.qty) AS shares FROM expenses AS ep JOIN users ON ep.user_id = users.id WHERE users.id = ? GROUP BY ep.symbol HAVING shares > 0 ORDER BY ep.symbol ASC", session["user_id"])
    user_id = session["user_id"]

    # Get user cash from database
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    user_cash = float(user_cash[0]["cash"])

    if request.method == "POST":

        # Get user stock from user-stock
        stock_symbol = request.form.get("symbol")

        # Check if stock and shares were submitted
        if not stock_symbol:
            return apology("please select stock", 400)

        try:
            input_shares = int(request.form.get("shares"))
        except (ValueError, TypeError):
            return apology("invalid input for number of shares", 400)

        if not input_shares or input_shares <= 0:
            return apology("invalid number of shares", 400)

        # Check if user owns the specified number of shares for the selected stock
        stock_exists = False
        for stock in user_stocks:
            if stock['symbol'] == stock_symbol:
                stock_exists = True
                if input_shares > stock['shares']:
                    return apology("you don't own that many shares of the stock", 400)
                break

        if not stock_exists:
            return apology("Invalid stock symbol", 400)

        # Look for current stock price
        result_quote = lookup(stock_symbol)

        # Create transection
        db.execute("INSERT INTO expenses (user_id, symbol, qty, price, datetime) VALUES (?, ?, ?, ?, ?)",
                   user_id, stock_symbol, -input_shares, float(result_quote["price"]), datetime.datetime.now())

        # Update user cash
        updated_user_cash = user_cash + (float(result_quote["price"]) * input_shares)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_user_cash, user_id)

        flash("Sold!!")
        return redirect("/")

    else:
        return render_template("sell.html", user_stocks=user_stocks)
