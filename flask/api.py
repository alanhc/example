from flask import Flask, request
from flask import render_template
from flask import jsonify
from data.test_json import data

"""example
curl -X POST --data "input=haha"  http://127.0.0.1:5000/post
curl -X POST -H "Content-Type: application/json" -d '{"status" : false, "name" : "Jack"}' "http://127.0.0.1:5000/post_json"
"""
app = Flask(__name__)



@app.route("/post", methods=['POST'])
def post():
    msg = request.values.get('input')
    return msg

@app.route("/post_json", methods=['POST'])
def post_json():
    data = request.get_json()
    return jsonify(data)


if __name__ == '__main__':
    app.debug = True
    app.run()


"""curl commend
curl: https://blog.techbridge.cc/2019/02/01/linux-curl-command-tutorial/

"""