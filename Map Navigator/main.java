import java.util.*;

public class MapNavigator {
    public static class Location{
        private String name;
        private Map<Location, Integer> neighbors;
        public Location(String name){
            this.name = name;
            this.neighbors = new HashMap<>();}
        public String getName(){
            return name;}
        public void addNeighbor(Location neighbor, int distance){
            neighbors.put(neighbor, distance);}
        public Map<Location, Integer> getNeighbors(){
            return neighbors;}}
            
    // Dijkstra's Algorithm
    public static Map<Location, Integer> findShortestPath(Location start, Location destination, Map<Location, Location> parent){
        Map<Location, Integer> distances = new HashMap<>();
        Set<Location> visited = new HashSet<>();
        for(Location location : start.neighbors.keySet()){
            distances.put(location, Integer.MAX_VALUE);}
        distances.put(start, 0);
        while(!visited.contains(destination)){
            Location minDistanceLocation = null;
            int minDistance = Integer.MAX_VALUE;
            for(Location location : distances.keySet()){
                if(!visited.contains(location) && distances.get(location) < minDistance){
                    minDistanceLocation = location;
                    minDistance = distances.get(location);}}
            if(minDistanceLocation==null){
                break;}
            visited.add(minDistanceLocation);
            for(Map.Entry<Location, Integer> neighbor : minDistanceLocation.getNeighbors().entrySet()){
                int newDistance = distances.get(minDistanceLocation) + neighbor.getValue();
                if(distances.getOrDefault(neighbor.getKey(), Integer.MAX_VALUE) > newDistance){
                    distances.put(neighbor.getKey(), newDistance);
                    parent.put(neighbor.getKey(), minDistanceLocation);}}}
        return distances;}

    public static List<Location> reconstructPath(Location start,Location destination,Map<Location,Location>parent){
        List<Location> path = new ArrayList<>();
        Location current = destination;
        while(current!=start){
            path.add(0, current);
            current = parent.get(current);}
        path.add(0, start);
        return path;}

    public static void main(String[] args){
        Location locationA = new Location("A");
        Location locationB = new Location("B");
        Location locationC = new Location("C");
        Location locationD = new Location("D");
        
        locationA.addNeighbor(locationB, 10);
        locationA.addNeighbor(locationC, 5);
        locationB.addNeighbor(locationD, 15);
        locationC.addNeighbor(locationD, 20);
        
        Map<Location, Location> parent = new HashMap<>();
        Map<Location, Integer> distances = findShortestPath(locationA, locationD, parent);
        if(distances.containsKey(locationD)){
            System.out.println("Shortest distance to destination: " + distances.get(locationD));
            List<Location> path = reconstructPath(locationA, locationD, parent);
            System.out.println("Shortest Path:");
            for(Location location : path){
                System.out.print(location.getName() + " -> ");}
            System.out.println("Destination");}
        else{
            System.out.println("No path found to destination.");}}}
