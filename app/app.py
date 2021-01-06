import os 
import csv
from flask import Flask, request, render_template, redirect, url_for, session

path = 'C:\\Users\\Landon\\Documents\\Arduino\\kinematics\\kinematics_flask\\data\\'
app = Flask(__name__)

@app.route('/')
def HomePage():
    filename = 'dummy.csv'
    return render_template('home_page.html')

@app.route('/', methods=['POST'])
def HomePage_UserData():
    state = True
    filename = create_filename(request.form)
    with open(path+filename, 'w') as File:
        writer = csv.writer(File)
        writer.writerow(['ax', 'ay', 'az','theta', 'beta', 'phi'])
    return redirect(url_for('IMU_logger'))

def create_filename(form):
    # landon_squats_1.csv
    return form['user_name']+'_'+form['workout_name']+'_'+form['set_number']+'.csv'

@app.route('/log', methods = ['GET'])
def IMU_logger():
    IMU_data = {key: float(val) for key, val in dict(request.args).items()}
    with open(path + filename, mode = 'a') as File:
        writer = csv.DictWriter(File, fieldnames = IMU_data.keys(), lineterminator = '\n')
        if state == True:
            writer.writerow(IMU_data)
    return render_template('log_page.html')

@app.route('/log', methods = ['POST'])
def IMU_Stop():
    state = False
    return redirect(url_for('HomePage'))

if __name__ == '__main__':
    global state, filename
    state = False
    filename = 'dummy.csv'
    app.run(host='192.168.1.109', port = 5010, debug=True)
    
    

    
