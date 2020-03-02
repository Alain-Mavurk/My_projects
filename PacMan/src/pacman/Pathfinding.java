package pacman;

import java.util.LinkedList;

public class Pathfinding {
	
	private Pathfinding() {
		
	}

	private static Noeud lowestFScore(LinkedList<Noeud> openlist, LinkedList<Noeud> closedlist) {
		if (openlist.size() == 1)
			return openlist.pop();
		else {
			Noeud current = openlist.getFirst();
			for (Noeud noeud : openlist) {
				if (current.f > noeud.f) {
					current = noeud;
				}
			}
			openlist.remove(current);
			return current;
		}
	}

	private static LinkedList<Noeud> adjacentCase(Noeud current, Coordinate destination, Game game) {
		int width = game.getWidth();
		int height = game.getHeight();
		LinkedList<Noeud> l = new LinkedList<Noeud>();
		int x = current.position.getY();
		int y = current.position.getX();
		if (x + 1 < height && !game.isWall(x+1, y))
			l.add(new Noeud(new Coordinate(y, x + 1), destination, current));
		if (x - 1 >= 0 && !game.isWall(x-1, y))
			l.add(new Noeud(new Coordinate(y, x - 1), destination, current));
		if (y + 1 < width && !game.isWall(x, y+1))
			l.add(new Noeud(new Coordinate(y + 1, x), destination, current));
		if (y - 1 >= 0 && !game.isWall(x, y-1))
			l.add(new Noeud(new Coordinate(y - 1, x), destination, current));
		return l;
	}

	private static boolean contains(LinkedList<Noeud> l, Coordinate position) {
		for (Noeud noeud : l) {
			if (noeud.position.equals(position))
				return true;
		}
		return false;
	}

	private static Noeud getNoeud(LinkedList<Noeud> l, Coordinate position) {
		if (contains(l, position)) {
			for (Noeud noeud : l) {
				if (noeud.position.equals(position))
					return noeud;
			}
		}
		return null;
	}

	private static void findPath(Coordinate start, Coordinate destination, Game game, LinkedList<Noeud> openlist, LinkedList<Noeud> closedlist) {

		openlist.add(new Noeud(start, destination, null));
		Noeud current;
		LinkedList<Noeud> adjacents = new LinkedList<Noeud>();

		do {
			current = lowestFScore(openlist, closedlist);
			closedlist.push(current);

			if (contains(closedlist, destination))
				break;

			adjacents = adjacentCase(current, destination, game);
			for (Noeud adjacent : adjacents) {
				if (contains(closedlist, adjacent.position))
					continue;

				if (!contains(openlist, adjacent.position))
					openlist.push(adjacent);
				else {
					Noeud oldAdjacent = getNoeud(openlist, adjacent.position);
					if (adjacent.f < oldAdjacent.f)
						oldAdjacent = adjacent;
				}
			}

		} while (!openlist.isEmpty());

	}
	
	public static LinkedList<Coordinate> getPath(Coordinate start, Coordinate destination, Game game){
		LinkedList<Noeud> openlist = new LinkedList<Noeud>();
		LinkedList<Noeud> closedlist = new LinkedList<Noeud>();
		findPath(start, destination, game, openlist, closedlist);
		LinkedList<Coordinate> path = new LinkedList<Coordinate>();
		Noeud current = closedlist.getFirst();
		while(current != null) {
			path.push(current.position);
			current = current.parent;
		}
		return path;
	}
}

class Noeud {
	public int f;
	public int g;
	public Coordinate position;
	public Noeud parent;

	public Noeud(Coordinate position, Coordinate destination, Noeud parent) {
		this.position = position;
		this.parent = parent;
		calculateF(destination);
	}

	public int calculateG() {
		if (parent == null)
			g = 0;
		else
			g = parent.g + 1;
		return g;
	}

	public int calculateH(Coordinate destination) {
		return Math.abs(position.getX() - destination.getX()) + Math.abs(position.getY() - destination.getY());
	}

	public void calculateF(Coordinate destination) {
		f = calculateG() + calculateH(destination);
	}

}
