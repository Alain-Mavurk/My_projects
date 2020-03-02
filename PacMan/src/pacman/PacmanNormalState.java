package pacman;


public class PacmanNormalState extends PacmanState {
	
	public PacmanNormalState(Pacman pacman) {
		super(pacman);
	}
	
	public void ghostCollision(Ghost ghost) {
		ghost.pacmanCollision(pacman);
	}

	@Override
	public void move() {
		pacman.wraparound();
		pacman.newPosition(
				(Pacman pacman) -> !pacman.getGame().ghostCollision(pacman.getPosition())
		);
	}

	@Override
	public State getState() {
		return State.NORMAL;
	}


}
