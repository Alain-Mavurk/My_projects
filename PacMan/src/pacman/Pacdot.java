package pacman;


public abstract class Pacdot extends Element{
	
	protected enum Type{
		NORMAL,
		INVISIBLE,
		SUPER,
		MAZE
	}
	
	protected Pacdot(Game game, Coordinate position) {
		super(game, position);
	}


	protected abstract int getPoints();
	
	protected abstract Type getType();
	
	protected abstract void eat();
	
}
