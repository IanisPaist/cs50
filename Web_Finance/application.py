import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    #select stocks a user owns
    stocks = db.execute("SELECT ticker, SUM(quantity) FROM transactions WHERE users_id=:u GROUP BY ticker",
                        u = session["user_id"])

    #we got list of dicts with two key-value pairs ticker and quantity
    total = 0
    for ticker in stocks:

        #first we need to find value of each ticker and add to dict
        stock_data = lookup(ticker["ticker"])
        ticker["current_price"] = stock_data["price"]

        #add total value of position
        ticker["total_value"] = ticker["SUM(quantity)"] * ticker["current_price"]

        #calculate grandtotal
        total = total + ticker["total_value"]

        #conver to usd format
        ticker["current_price"] = usd(ticker["current_price"])
        ticker["total_value"] = usd(ticker["total_value"])

    #also get users cash balance from users.db
    balance = db.execute("SELECT cash FROM users WHERE id=:u",
                u = session["user_id"])

    cash = balance[0]["cash"]
    #calculate total value with cash
    total = total + cash

    #convert to usd
    cash = usd(cash)
    total =usd(total)

    return render_template("index.html", stocks = stocks, cash = cash, total=total)

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    if request.method == "POST":

        #check that cash is positive
        if float(request.form.get("cash")) < 0:
            return apology("Enter valid amount", 403)

        #get how much user have now
        balance = db.execute("SELECT cash FROM users WHERE id=:i",
                            i = session["user_id"])

        cur_balance = float(balance[0]["cash"])

        db.execute("UPDATE users SET cash=:c WHERE id=:i",
                    c = float(request.form.get("cash")) + cur_balance,
                    i = session["user_id"]
                    )
        # Redirect user to login form
        return redirect("/")

    else:
        return render_template("cash.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    #if user posts form
    if request.method == "POST":

        #find stock data
        result = lookup(request.form.get("symbol"))

        #check field symbol that it is not blank and symbol exists
        if not request.form.get("symbol") or result == None:
            return apology("Please provide valid stock ticker", 403)

        #check number of shares
        if not request.form.get("shares").isnumeric() or int(request.form.get("shares")) < 0:
            return apology("Please provide correct number", 403)

        #get users balance
        balance = db.execute("SELECT cash FROM users WHERE id=:user_id",
                            user_id = session["user_id"])

        #write balance to variable
        cash = balance[0]["cash"]

        #calculate purchase value
        purch_value = float(result["price"]) * float(request.form.get("shares"))

        #check if cash is enough
        if purch_value > cash:
            return apology("Not enough cash", 403)

        #insert info about purchase into transactions.db
        db.execute("INSERT INTO transactions (users_id, ticker, transType, purch_value, quantity) VALUES (:u, :t, :tt, :p, :q)",
                    u = session["user_id"],
                    t = request.form.get("symbol"),
                    tt = 1,
                    p = purch_value,
                    q = request.form.get("shares")
                    )


        #change amount of cash after purchase
        db.execute("UPDATE users SET cash=:upd_cash WHERE id=:user_id",
                    upd_cash = cash - purch_value,
                    user_id = session["user_id"])

        # Redirect user to login form
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT * FROM transactions WHERE users_id=:u",
                            u = session["user_id"])

    for t in transactions:
        t["purch_value"] = usd((-1 * t["purch_value"]))


    return render_template("history.html", transactions = transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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
    """Get stock quote."""
    #if user submites the form
    if request.method == "POST":
        result = lookup(request.form.get("symbol"))
        #format price to usd
        result["price"] = usd(result["price"])

        return render_template("quoted.html", result = result)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        #check that username is provided
        if not request.form.get("username"):
            return apology("must provide username", 403)

        #check that password is provided
        if not request.form.get("password"):
            return apology("must provide password", 403)

        #check that confirmation password is provided
        if not request.form.get("confirmation"):
            return apology("must provide confirmation password", 403)

        #check that password and confirmation password match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 403)

        # Query database for username
        usr = db.execute("SELECT * FROM users WHERE username = :username",
                        username=request.form.get("username"))

        #if result is not empty that means that that username already exists but we need unique
        if len(usr) != 0:
            return apology("Username already exists", 403)

        #if everything is ok - registeruser
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashed_pwd)",
                    username = request.form.get("username"),
                    hashed_pwd = generate_password_hash(request.form.get("password")))
        # Redirect user to login page
        return redirect("/login")
    else:
        # User reached route via GET (as by clicking a link or via redirect)
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        #check symbol exists
        if not request.form.get("symbol"):
            return apology("Please enter ticker", 403)

        #check that user has this many stocks
        tickers = db.execute("SELECT SUM(quantity) FROM transactions WHERE users_id=:u AND ticker=:t GROUP BY ticker",
                            u = session["user_id"],
                            t = request.form.get("symbol")
                            )

        #return apology if not enough shares
        if int(tickers[0]["SUM(quantity)"]) < int(request.form.get("shares")):
            return apology("You don't have this amount", 403)

        #return apology if user provides negative number or zero
        if int(request.form.get("shares")) < 1:
            return apology("Please enter positive number of shares")

        #calculate market price of transaction
        result = lookup(request.form.get("symbol"))

        mktvalue = -1 * (result["price"] * int(request.form.get("shares")))

        #insert data about the sale into transactions
        db.execute("INSERT INTO transactions (users_id, ticker, transType, purch_value, quantity) VALUES (:u, :t, :tt, :p, :q)",
                    u = session["user_id"],
                    t = request.form.get("symbol"),
                    tt = 0,
                    p = mktvalue,
                    q = -1 * int(request.form.get("shares"))
                    )

        #update users cash balance
        rows = db.execute("SELECT cash FROM users WHERE id=:u",
                        u = session["user_id"])

        current_balance = rows[0]["cash"]

        cash_received = mktvalue * -1

        db.execute("UPDATE users SET cash=:cash WHERE id=:u",
                    cash = current_balance + cash_received,
                    u = session["user_id"]
                    )

        #return to homepage
        return redirect("/")

    else:
        #return current portfolio
        tickers = db.execute("SELECT ticker, SUM(quantity) FROM transactions WHERE users_id=:u GROUP BY ticker",
                    u = session["user_id"])

        return render_template("sell.html", tickers = tickers)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
