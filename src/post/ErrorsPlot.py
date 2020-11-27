#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Nov 26 01:46:48 2020

@author: aziz
"""

import numpy as np
import matplotlib.pyplot as plt


def readError(file="/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/Erreurs.out"):

    Header = ("Itérations", "Error rho", "Error rhoU",
              "Error rhoV", "Error rhoE")
    Erreurs = np.loadtxt(file, skiprows=3)
    fig = plt.figure()
    ax = fig.add_subplot(111)
    plt.title("Courbes de convergence des résidus")
    legend = []

    abcisse = Header[0]
    ordonnee = "Erreurs"
    x_data = Erreurs[:, 0]
    for i in range(1, len(Header)):
        y_data = Erreurs[:, i]
        ax.plot(x_data, y_data, '.')
        legend.append(Header[i])

    ax.set_yscale('log')
    # ax.set_xscale('log')
    # ax.grid(b=True, which='minor', color='grey', linestyle='--')
    # ax.grid(b=True, which='major', color='k', linestyle='-')
    plt.xlabel(abcisse)
    plt.ylabel(ordonnee)
    plt.legend(legend)
    plt.savefig('Residus_Courbes.png', dpi=900)
    plt.show()

    return


def plotAerodynamiques(file="/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/Aerodynamique.out"):

    Header = ("x/c", "Cp")
    x_Cp = np.loadtxt(file, skiprows=4)
    fig = plt.figure()
    ax = fig.add_subplot(111)
    plt.title("Courbe Cp vs x/c")
    abcisse = Header[0]
    ordonnee = Header[1]
    x_data = x_Cp[:, 0]
    y_data = x_Cp[:, 1]
    ax.plot(x_data, y_data, '.')
    plt.gca().invert_yaxis()
    plt.xlabel(abcisse)
    plt.ylabel(ordonnee)
    plt.savefig('Cp_vs_x.png', dpi=900)
    plt.show()
    return


def main():
    """Principal function"""

    readError()
    plotAerodynamiques()

    return


if __name__ == '__main__':

    main()
