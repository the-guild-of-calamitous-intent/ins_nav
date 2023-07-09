##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
import numpy as np

try:
    import matplotlib.pyplot as plt

    def magplot(data):
        """
        Plot magnetometer data
        """
        x = [v[0] for v in data]
        rx = (max(x)-min(x))/2
        cx = min(x)+rx

        y = [v[1] for v in data]
        ry = (max(y)-min(y))/2
        cy = min(y)+ry

        z = [v[2] for v in data]
        rz = (max(z)-min(z))/2
        cz = min(z)+rz

        alpha = 0.5
        u = np.linspace(0, 2 * np.pi, 100)

        plt.plot(rx*np.cos(u)+cx, ry*np.sin(u)+cy,'-r',label='xy')
        plt.plot(x,y,'.r',alpha=alpha)

        plt.plot(rx*np.cos(u)+cx, rz*np.sin(u)+cz,'-g',label='xz')
        plt.plot(x,z,'.g',alpha=alpha)

        plt.plot(rz*np.cos(u)+cz, ry*np.sin(u)+cy,'-b',label='zy')
        plt.plot(z,y, '.b',alpha=alpha)

        plt.title(f"CM:({cx:.1f}, {cy:.1f}, {cz:.1f}) uT  R:({rx:.1f}, {ry:.1f}, {rz:.1f}) uT")
        plt.xlabel('$\mu$T')
        plt.ylabel('$\mu$T')
        plt.grid(True);
        plt.axis('equal')
        plt.legend();


    class MagPlot:
        """
        Real-time plotting of magnetometer values useful during calibration
        data collection.
        """
        def __init__(self, BUFFER_SIZE=10000):
            self.fig, self.ax = plt.subplots(1, 1)
            self.ax.set_aspect(1)

            self.mag_x = deque(maxlen=BUFFER_SIZE)
            self.mag_y = deque(maxlen=BUFFER_SIZE)
            self.mag_z = deque(maxlen=BUFFER_SIZE)

        def push(self, x,y,z):
            # save data for real-time plotting
            self.mag_x.append(x)
            self.mag_y.append(y)
            self.mag_z.append(z)

        def plot(self, title=None):
            # Clear all axis
            self.ax.cla()
            x = self.mag_x
            y = self.mag_y
            z = self.mag_z

            # Display the sub-plots
            self.ax.scatter(x, y, color='r', label="X-Y")
            self.ax.scatter(y, z, color='g', label="Y-Z")
            self.ax.scatter(z, x, color='b', label="Z-X")
            self.ax.grid()
            self.ax.legend()

            if title is None:
                title = "MagPlot"
            self.ax.set_title(title)

            # Pause the plot for INTERVAL seconds
            plt.pause(0.01)

except ImportError:
    print("Please install matplotlib to use")

    class MagPlot:
        pass

    def magplot(data):
        pass


def magcal(Bp, uT=None):
    """
    Modelled after the matlab function: magcal(D) -> A, b, expmfs
    inputs:
        Bp: data points
        uT: expected field strength for longitude/altitude. If None
            is given, then automatically calculated and used
    returns:
        A: soft-iron 3x3 matrix of scaling
        b: hard-iron offsets
        expmfs: expected field strength
    """
    Y = np.array([v[0]**2+v[1]**2+v[2]**2 for v in Bp])
    X = np.hstack((Bp,np.ones((Bp.shape[0],1))))
    beta = np.linalg.inv(X.T.dot(X)).dot(X.T.dot(Y))
    b=0.5*beta[:3]

    # expected mag field strength
    expmfs=np.sqrt(beta[3]+b[0]**2+b[1]**2+b[2]**2)

    if uT is None:
        uT = expmfs

    x = [v[0] for v in Bp]
    rx = (max(x)-min(x))/2

    y = [v[1] for v in Bp]
    ry = (max(y)-min(y))/2

    z = [v[2] for v in Bp]
    rz = (max(z)-min(z))/2

    A = np.diag([uT/rx,uT/ry,uT/rz])
    return A,b,expmfs
