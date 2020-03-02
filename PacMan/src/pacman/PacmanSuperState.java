package pacman;

public class PacmanSuperState extends PacmanState {

	private static final int TIMEOUT = 75;

	public PacmanSuperState(Pacman pacman) {
		super(pacman);
		timer = TIMEOUT;
	}

	@Override
	public void move() {
		pacman.wraparound();
		pacman.newPosition(
				(Pacman pacman) -> !pacman.getGame().ghostCollision(pacman.getPosition())
		);
		timerDecrease();
		checkTimeout(State.NORMAL);
	}
	
	@Override
	public State getState() {
		return State.SUPERPACMAN;
	}

	@Override
	protected void ghostCollision(Ghost ghost) {
		ghost.pacmanCollision(pacman);
	}

}
