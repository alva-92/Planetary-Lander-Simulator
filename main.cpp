/**
 * Copyright (c) 2019 Gerardo Enrique Alvarenga.
 *
 * This work is licensed under the terms of the MIT license.
 *
 *
 *  @name         main.cpp
 *  @description  The planetary lander is a game which uses differential equations (ODEs) 
 *                and solves them in real-time in order to simulate the lander getting to 
 *                a planet. Keeps highscores of players.
 *  @author       Gerardo Enrique Alvarenga
 *  @version      1.1
 */

#ifdef _MSC_VER
#include <crtdbg.h>
#else
#define _ASSERT(expr) ((void)0)
#define _ASSERTE(expr) ((void)0)
#endif

#include <stdio.h> 
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#include <conio.h> 
#else
#include <curses.h>
#endif
 
    /* Simulation Constants */
const double gravity = 3.7;
const double c       = 0.13;
const double a       = 8.3;
const double vMax    = 46;
#define FUEL_RESERVE 100;
#define INCREASE_B   'W'
#define DECREASE_B   'E'
#define EMPTY_RESERVE 0

    /* Function Declarations */
void runSimulation();
void display_stored_data();
void reRunSimulation();
bool sortSimulationTime(const std::tuple<std::string, double, double>&, std::tuple <std::string, double, double>&);
bool sortSimulationBounces(const std::tuple<std::string, double, double>&, std::tuple <std::string, double, double>&);

/**
 * Initialize the program and call the required functions in the appropriate sequence
 */
int main() {
	char menuSelection;
	bool isSimulationActive = true;
	while (isSimulationActive) { /* Run menu idefinitely while the user wishes to continue */
		std::cout << "\tFlight Simulator\n" << std::endl;
		std::cout << "Menu" << std::endl;
		std::cout << " 1 - Run Simulation" << std::endl; 
		std::cout << " 2 - Display simulation data" << std::endl; 
		std::cout << " 3 - Quit" << std::endl;
		std::cin >> menuSelection;
		switch (menuSelection) {
			case '1':
				runSimulation();
				break;
			case '2':
				system("CLS");   /* Clear the console screen before starting the simulation */
				display_stored_data();
				break;
			case '3':
				system("CLS");
				std::cout << "Terminating application..." << std::endl;
				isSimulationActive = false; /* Break the loop - Terminate application */
				break;
			default:
				std::cout << "Invalid Selection. Select a value from the options shown above" << std::endl;
				break;
		}
	}
	return 0;
}

void runSimulation() {
	system("CLS");                      /* Clear the console screen before starting the simulation */
	double dv_dt = 0.0;                 /* Acceleration of the lander module as it falls to the surface */
	double dv_dtPrime = 0.0;
	double v = 0.0;                     /* Velocity (metres/second). Positive in a downward direction. */
	double vPrime = 0.0;
	double fuelReserve = FUEL_RESERVE;  /* Fixed reserve of fuel. Usage has a rate of B/s */
	double heightAboveSurface = 1000;   /* Lander is 1000 meters above the surface when released from the mother ship*/
	double currentPosition = 1000;      /* Position of the lander. Initialized at the height above the surface */
	double burntFuel = 0.0;             /* Burnt fuel count */
	double miliseconds = 0.0;
	double seconds = 0.0;
	double minutes = 0.0;
	double bounceCounter = 0.0;          /* Number of the lander bounces */
	double changeTime = 0.0;
	char inputKey = '\0';
	double startTime = GetTickCount();

	bool isDescentInProgress = true;
	while (isDescentInProgress) {
		std::cout << "****************************** STARTING DESCENT ******************************" << std::endl;
		changeTime = GetTickCount() - startTime;
		startTime += changeTime;
		miliseconds += changeTime;
		
		if (miliseconds >= 1000) { 
			miliseconds = 0; 
			seconds++; 
		}
		if (seconds >= 60) { 
			seconds = 0; 
			minutes++; 
		}
		
		if (_kbhit()) { /* Check user's input */
			inputKey = _getch(); /* Retrieve character */
			if (inputKey == INCREASE_B && fuelReserve > EMPTY_RESERVE)
				burntFuel += 1;
			if (inputKey == DECREASE_B && burntFuel > EMPTY_RESERVE) 
				burntFuel -= 1;
		}
		if (burntFuel > 0)
			fuelReserve -= (burntFuel * (changeTime/1000));
		if (fuelReserve <= 0) { 
			burntFuel = 0.0; 
			fuelReserve = 0.0; 
		}

		dv_dt = gravity - c*(v + a * pow((v / vMax), 3)) - burntFuel;
		vPrime = v + dv_dt * (changeTime / 1000);
		dv_dtPrime = gravity - c * (vPrime + a * pow((vPrime / vMax), 3)) - burntFuel;
		v = v + ((dv_dt + dv_dtPrime) / 2) * (changeTime / 1000);

		heightAboveSurface = heightAboveSurface - (v*(changeTime / 1000));

		if (heightAboveSurface < 1 && v > 1) {	
			bounceCounter++;
			v = 0 - v;
		}
		else if (inputKey == DECREASE_B)
			std::cout << "E was Pressed - Reduce burn ";
		else if (inputKey == INCREASE_B)
			std::cout << "W was Pressed - Increase burn ";
		std::cout << ": BURN = " << burntFuel << "; " << "BURN RESERVE = " << fuelReserve << std::endl;
		std::cout << "MINUTE = " << minutes << "; " << "SECOND = " << seconds << "; " << "MILLISECOND = " << miliseconds << std::endl;
		std::cout << "SPEED OF DESCENT = " << v << "m/s" << std::endl;
		std::cout << "HEIGHT ABOVE MARTIAN SURFACE = " << heightAboveSurface << "metres" << std::endl;
		std::cout << "NUMBER OF BOUNCES = " << bounceCounter << std::endl;
		Sleep(200);
		system("CLS");

		if (heightAboveSurface < 1 && v < 1 && v > 0) { /* Check if the landing was successful. */
			std::ofstream simulationData;
			simulationData.open("../SimulationData/simulationData.txt", std::ios_base::app);
			if (!simulationData.is_open()) {
				simulationData.close();
				std::cout << "Error opening file.\n";
				return;
			}
			std::string entryName;
			std::cout << "Enter a name for this simulation entry: ";
			std::cin >> entryName;

			double scoreMinutes = minutes * 60;
			double scoreMiliseconds = (miliseconds / 1000);
			double scoreSeconds = (scoreMinutes + scoreMiliseconds + seconds);

			simulationData << "\n" << entryName << " " << bounceCounter << " " << scoreSeconds; /* Write the score on file */
			simulationData.close();
			system("CLS");
			reRunSimulation(); /* Check if the user wishes to run the simulation once again */
			isDescentInProgress = false;
		}
	}
}

/**
 * Prompts the user whether the simulation should run again.
 */
void reRunSimulation() {
	char reRunSimulation;
	bool isRunningAgain = true;
	while (isRunningAgain) { /* Loop to prompt the user for a re-run  */
		std::cout << "Play again? (y/n)" << std::endl;
		std::cout << "Press y to run the simulation again, or press n to go back to main menu. " << std::endl;
		std::cin >> reRunSimulation;
		switch (reRunSimulation) {
		case 'y':
			runSimulation();
			break;
		case 'n':
			system("CLS");
			isRunningAgain = false;
			break;
		default:
			std::cout << "Not a valid selection. Enter (y/n)" << std::endl;
			break;
		}
	}
}

/**
 * Display the information saved from the simulations to the console
 */
void display_stored_data() {
	system("CLS");                                                 /* Clear the console screen before displaying the information */
	std::vector<std::tuple <std::string, double, double>> entries; /* Create a vector to hold the information of the simulation */
	std::ifstream simulationData;
	simulationData.open("../SimulationData/simulationData.txt");
	if (!simulationData.is_open()) {
		simulationData.close();
		std::cout << "Error opening file.\n";
		return;
	}
	std::string name;
	double bounces;
	double seconds;
	while (simulationData >> name >> bounces >> seconds)
		entries.push_back(make_tuple(name, bounces, seconds));
	sort(entries.begin(), entries.end(), sortSimulationBounces);
	sort(entries.begin(), entries.end(), sortSimulationTime);
	std::cout << "*************************** Simulations Data ***************************" << std::endl;
	std::cout << "Entries" << std::endl;
	for (const auto& i : entries)
		std::cout << "Name: " << std::get<0>(i) << " \tBounces:" << std::get<1>(i) << " \tTime(Seconds): " << std::get<2>(i) << " " << std::endl;
	simulationData.close();
	std::cout << "\nPress any key to continue" << std::endl;
	_getch();
	system("CLS");
}

/**
 * Compare the times in the entries located in the 2nd position of the tuple
 * 
 * @param const std::tuple<std::string, double, double> &tuple1 
 * @param std::tuple <std::string, double, double> &tuple2
 *
 * @return Boolean value of the comparison
 */
bool sortSimulationBounces(const std::tuple<std::string, double, double> &tuple1, std::tuple <std::string, double, double> &tuple2) {
	return std::get<1>(tuple1) < std::get<1>(tuple2);
}

/**
 * Compare the times in the entries located in the 3rd position of the tuple
 * 
 * @param const std::tuple<std::string, double, double> &tuple1
 * @param std::tuple <std::string, double, double> &tuple2
 * 
 * @return Boolean value of the comparison
 */
bool sortSimulationTime(const std::tuple<std::string, double, double> &tuple1, std::tuple <std::string, double, double> &tuple2) {
	return std::get<2>(tuple1) < std::get<2>(tuple2); 
}