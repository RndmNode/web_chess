from flask import Flask, render_template, url_for, request

app = Flask(__name__)

# home page
@app.route('/', methods = ['GET'])
def home():
    return render_template('chessPage.html')

# every time a move is made, this is called to get move information
@app.route('/make_move', methods = ['POST'])
def makeMove():
    move = request.form.get('move')
    return {'move' : move}

# main 
def main():
    app.run(port=5050, debug=True, threaded=True)

# run
if __name__ == '__main__':
    main()