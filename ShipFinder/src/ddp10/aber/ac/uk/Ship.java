package ddp10.aber.ac.uk;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class Ship {
 
	private String mmsi;
	private String name;
	private Float lat;
	private Float lng;
	private Integer course;
	private Float speed;

	public Ship(String mmsi, String name, Float lat, Float lng, Integer course,
			Float speed) {
		this.mmsi = mmsi;
		this.name = name;
		this.lat = lat;
		this.lng = lng;
		this.course = course;
		this.speed = speed;
	}

	protected Ship(File shipFile) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader(shipFile));
		this.mmsi = br.readLine();
		this.name = br.readLine();
		this.lat = Float.valueOf(br.readLine());
		this.lng = Float.valueOf(br.readLine());
		this.course = Integer.valueOf(br.readLine());
		this.speed = Float.valueOf(br.readLine());
		br.close();
	}

	public String toString() {
		StringBuffer ret = new StringBuffer();
		ret.append(mmsi);
		ret.append("\n");
		ret.append(name);
		ret.append("\n");
		ret.append(lat);
		ret.append("\n");
		ret.append(lng);
		ret.append("\n");
		ret.append(course);
		ret.append("\n");
		ret.append(speed);
		return ret.toString();
	}

	public Boolean matches(String searchTerm, int field) {
		Boolean ret = false;
		if (0 == field && mmsi.contains(searchTerm)) {
			ret = true;
		} else if (1 == field && name.contains(searchTerm)){
			ret = true;
		}
		return ret;
	}
}
