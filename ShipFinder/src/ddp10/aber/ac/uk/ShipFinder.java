package ddp10.aber.ac.uk;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Scanner;
import java.util.Vector;
import java.io.FileOutputStream;
import java.nio.channels.*;

public class ShipFinder {

	/**
	 * @param args
	 */
	private Scanner scan = new Scanner(System.in);

	private void runShipFinding() {
		while (true) {
			System.out.println("Do you want to search for MMSI or ship Name?"
					+ " type \"MMSI\" \"Name\" or \"q\" to quit");
			String choice = scan.nextLine();
			if (choice.equalsIgnoreCase("q")) {
				break;
			} else if (choice.equalsIgnoreCase("mmsi")) {
				System.out.println("please enter the MMSI of the ship");
				Integer searchField = new Integer(0);
				search(searchField);
			} else if (choice.equalsIgnoreCase("name")) {
				System.out.println("please enter the name of the ship");
				Integer searchField = new Integer(1);
				search(searchField);
			}
		}
	}

	private void search(Integer searchField) {
		String searchTerm; // search for ships matching search term
		searchTerm = scan.nextLine();
		try {
			FileOutputStream fos = new FileOutputStream("log.txt", true);
			FileLock fl = fos.getChannel().tryLock();
			if (fl != null) {
				Vector<Ship> ships = searchWorkingDir(searchTerm, searchField);
				for (Ship ship : ships) { // print out information about ships
					System.out.println(ship.toString());
					FileWriter fw = new FileWriter(fos.getFD());
					DateFormat dateFormat = new SimpleDateFormat("EEE MMM DD HH:mm:ss yyyy");
					Date date = new Date();
					StringBuffer logRecord = new StringBuffer(dateFormat.format(date));
					logRecord.append("\n Search was made for ships with ");
					if (searchField == 0){
						logRecord.append("mmsi of");
					}else {
						logRecord.append("name of");
					}
					logRecord.append(searchTerm);
					logRecord.append("\n");
					fw.write(logRecord.toString());
					fl.release();
					fw.close();
				}
			}else System.out.println("Another program is accessing ship files");
			
		} catch (IOException e) {

		}
	}

	/**
	 * 
	 * @param searchTerm
	 * @param field
	 *            0 for MMSI 1 for name
	 * @return
	 */
	private Vector<Ship> searchWorkingDir(String searchTerm, int field) {
		Vector<Ship> foundShips = new Vector<Ship>(10, 2); // vector to store
															// results of search
		File workingDir = new File(System.getProperty("user.dir"));
		String[] children = workingDir.list();
		for (String s : children) { // for each file
			if (s.matches(".\\.shp")) { // check file is a ship file
				File shipFile = new File(s); // try to read in file
				try {
					Ship ship = new Ship(shipFile);
					if (ship.matches(searchTerm, field)) {// save result if ship
						foundShips.add(ship); // matches search term
					}
				} catch (FileNotFoundException e) {
					System.err.println("filcheck e not found");
				} catch (IOException e) {
					System.err.println("error reading file");
				}
			}
		}
		return foundShips;
	}

	public static void main(String[] args) {
		ShipFinder finder = new ShipFinder();
		finder.runShipFinding();
	}

}
