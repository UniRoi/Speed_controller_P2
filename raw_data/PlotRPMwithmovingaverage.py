import numpy as np
import matplotlib.pyplot as plt


# Function to read RPM data from the text file
def read_data(filename):
    rpm_values = []

    with open(filename, 'r') as file:
        for line in file:
            parts = line.strip().split()
            if len(parts) >= 1:
                try:
                    rpm = float(parts[0])  # First value: RPM
                    rpm_values.append(rpm)
                except ValueError:
                    print(f"Invalid data encountered: {line.strip()}")

    return rpm_values


# Moving average function
def moving_average(data, window_size):
    """Calculate the moving average of a given data list."""
    return np.convolve(data, np.ones(window_size) / window_size, mode='valid')


# Function to plot the RPM data
def plot_rpm(rpm_values, filtered_values, window_size):
    time_values = [i * 0.1 for i in range(len(rpm_values))]  # Time in seconds (100ms intervals)

    plt.figure(figsize=(10, 5))
    plt.plot(time_values, rpm_values, label='Original RPM', color='red', alpha=0.5)
    plt.plot(time_values[window_size - 1:], filtered_values,
             label='Moving Average (window size: {})'.format(window_size), color='blue')
    plt.title('RPM Over Time with Moving Average')
    plt.ylabel('RPM')
    plt.xlabel('Time (s)')
    plt.grid()
    plt.legend()
    plt.tight_layout()  # Adjusts the layout to fit in the figure area.
    plt.show()


# Main function
if __name__ == "__main__":
    filename = 'bla.txt'  # Replace with your actual file name
    rpm_values = read_data(filename)

    # Define the window size for the moving average
    window_size = 10  # You can change this value for more or less smoothing

    # Apply the moving average
    filtered_values = moving_average(rpm_values, window_size)

    # Plot the original and filtered RPM values
    plot_rpm(rpm_values, filtered_values, window_size)
