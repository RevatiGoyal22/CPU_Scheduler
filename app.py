from flask import Flask, render_template, request
import subprocess
import json

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/schedule', methods=['POST'])
def schedule():
    type = request.form.get('type')
    n = request.form.get('n')
    processes = []
    for i in range(int(n)):
        arrivalTime = request.form.get(f'arrivalTime{i+1}')
        burstTime = request.form.get(f'burstTime{i+1}')
        processes.append((arrivalTime, burstTime))
    timeQuantum = request.form.get('timeQuantum') if type == '4' else None


    try:
        # Write input to input.txt
        with open('input.txt', 'w') as f:
            f.write(f"{type}\n{n}\n")
            for arrivalTime, burstTime in processes:
                f.write(f"{arrivalTime} {burstTime}\n")
            if timeQuantum:
                f.write(f"{timeQuantum}\n")

        # Run the C++ executable
        result = subprocess.run(['./scheduler'], capture_output=True, text=True)

        # Check if executable ran successfully
        if result.returncode != 0:
            return f"Error running scheduler: {result.stderr}", 500
    except Exception as e:
        return f"An error occurred: {str(e)}", 500
    

    with open('output.txt', 'r') as f:
        output_data = f.read().splitlines()
    processes_output = []

    # Debugging output_data content
    print("Output data lines:")
    for line in output_data:
        print(line)

    for line in output_data[:-3]:  
        parts = line.split()
        if len(parts) == 7:
            num,arrival_time, burst_time, completion_time, turnaround_time, waiting_time, response_time = parts
            processes_output.append({
                "num":num,
                "arrival_time": arrival_time,
                "burst_time": burst_time,
                "completion_time": completion_time,
                "turnaround_time": turnaround_time,
                "waiting_time": waiting_time,
                "response_time": response_time
            })

    avg_response_time = float(output_data[-3])
    avg_waiting_time = float(output_data[-2])
    avg_turnaround_time = float(output_data[-1])

    return render_template(
        'result.html',
        avg_turnaround_time=avg_turnaround_time,
        avg_waiting_time=avg_waiting_time,
        avg_response_time=avg_response_time,
        processes=processes_output
    )
    
if __name__ == '__main__':
    app.run(debug=True)
