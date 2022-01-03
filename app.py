from flask import Flask, render_template, url_for, request, make_response, jsonify
from service.external.client import getAI_move

app = Flask(__name__)

# home page
@app.route('/', methods = ['GET'])
def home():
    return render_template('chessPage.html')

# every time a move is made, this is called to get move information
@app.route('/make_move', methods = ['POST'])
def make_move():
    FEN = request.form.get('fen')
    depth = int(request.form.get('fixed_depth'))
    AI_move = getAI_move(FEN, depth)
    while AI_move == 'a8a8':
        AI_move = getAI_move(FEN)
        continue
    return {'move' : AI_move.move}

# main 
def main():
    app.run(port=5000, debug=True, threaded=True)

# run
if __name__ == '__main__':
    main()