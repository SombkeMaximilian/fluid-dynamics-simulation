import os
import numpy as np
import matplotlib.pyplot as plt

def read_data(file = 'vec.bin'):
    with open(file, 'rb') as f:
        data = f.read()
    return np.frombuffer(data, dtype=np.float64).copy()


def transform_data(data):
    data_length = len(data) // 2
    side_length = int(np.sqrt(data_length))
    paired_data = data.reshape((data_length, 2))
    square_array = paired_data.reshape((side_length, side_length, 2))
    return square_array


def create_stream_plot(data, plot_file):
    side_length = data.shape[0]
    x_i = np.linspace(0, side_length - 1, side_length)
    y_i = np.linspace(0, side_length - 1, side_length)
    x, y = np.meshgrid(x_i, y_i)
    u = data[:, :, 0]
    v = data[:, :, 1]
    magnitude = np.sqrt(u**2 + v**2)

    u[0, :] = np.nan
    u[-1, :] = np.nan
    u[:, 0] = np.nan
    u[:, -1] = np.nan
    v[0, :] = np.nan
    v[-1, :] = np.nan
    v[:, 0] = np.nan
    v[:, -1] = np.nan

    color_map = plt.get_cmap('turbo')

    fig, ax = plt.subplots()
    stream = ax.streamplot(x, y, u, v, density = 1.7, color = magnitude, cmap = color_map,
                           linewidth = 1.2, arrowsize = 1.5, arrowstyle = 'fancy', zorder = 1)
    fig.colorbar(stream.lines)

    ax.set_title('Stream Plot of Flow Velocity')
    ax.set_xlabel('')
    ax.set_ylabel('')
    ax.set_xlim(x_i[0], x_i[-1])
    ax.set_ylim(y_i[0], y_i[-1])
    for side in ['top', 'right', 'bottom', 'left']:
        ax.spines[side].set_visible(True)
        ax.spines[side].set_linewidth(1)
    ax.xaxis.set_ticks_position('bottom')
    ax.yaxis.set_ticks_position('left')

    plt.savefig(plot_file, dpi = 1000, bbox_inches = 'tight',
                pad_inches = 0.1, format = 'png')
    plt.close(fig)


def main():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    os.chdir(script_dir)
    os.makedirs('plots', exist_ok = True)
    bin_files = [f for f in os.listdir('.') if f.endswith('.bin')]
    for bin_file in bin_files:
        data = read_data(bin_file)
        transformed_data = transform_data(data)
        plot_file = os.path.join('plots', bin_file.replace('.bin', '.png'))
        create_stream_plot(transformed_data, plot_file)


if __name__ == '__main__':
    main()
