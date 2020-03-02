package pacman;


public class PacdotMaze extends Pacdot {

	public PacdotMaze(Game game, Coordinate position) {
		super(game, position);
	}


	@Override
	public void eat() {
		game.getMaze().generateNewBoard();
		game.incScore(this);
		game.removePacdot(this);
	}

	@Override
	public int getPoints() {
		return 1000;
	}


	@Override
	public Type getType() {
		return Type.MAZE;
	}

}
