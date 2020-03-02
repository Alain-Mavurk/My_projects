package pacman;


public class PacdotNormal extends Pacdot {

	public PacdotNormal(Game game, Coordinate position) {
		super(game, position);
	}

	@Override
	public void eat() {
		game.incScore(this);
		game.removePacdot(this);
	}

	@Override
	public int getPoints() {
		return 100;
	}

	@Override
	public Type getType() {
		return Type.NORMAL;
	}

}
