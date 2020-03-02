package pacman;


public class PacmanInvisibleState extends PacmanState {
	private static final int TIMEOUT = 75;

	public PacmanInvisibleState(Pacman pacman) {
		super(pacman);
		timer = TIMEOUT;
	}

	@Override
	public void move() {
		pacman.wraparound();
		pacman.newPosition(
				(Pacman pacman) -> !pacman.getGame().isOut(pacman.nextPosition())
		);
		timerDecrease();
		checkTimeout(State.NORMAL);
	}

	@Override
	public State getState() {
		return State.INVISIBLE;
	}
		
	@Override
	protected void ghostCollision(Ghost ghost) {
		
	}


}
