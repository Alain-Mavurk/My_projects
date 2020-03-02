package pacman;

public class Element {
	protected Coordinate position;
	protected Game game;
	
	public Element(Game game, Coordinate start) {
		this.game = game;
		position = start;
	}
	
	
	
	protected Coordinate getPosition() {
		return position;
	}
	
	
	
	protected Game getGame() {
		return game;
	}
}
