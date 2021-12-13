from flask import Flask, render_template, url_for, request, make_response, jsonify
from service.external.src.client import getAI_move

app = Flask(__name__)

# home page
@app.route('/', methods = ['GET'])
def home():
    return render_template('chessPage.html')

# every time a move is made, this is called to get move information
@app.route('/make_move', methods = ['POST'])
def make_move():
    FEN = request.form.get('fen')
    AI_move = getAI_move(FEN)
    print(AI_move.fen)
    return {'fen' : AI_move.fen}

# main 
def main():
    app.run(port=5000, debug=True, threaded=True)

# run
if __name__ == '__main__':
    main()