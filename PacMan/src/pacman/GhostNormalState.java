package pacman;


public class GhostNormalState extends GhostState {

	public GhostNormalState(Ghost ghost) {
		super(ghost);
	}

	@Override
	protected void move() {
		ghost.newPosition();
	}

	@Override
	protected void pacmanCollision(Pacman pacman) {
		ghost.getGame().reset();
	}

	@Override
	protected State getState() {
		return State.NORMAL;
	}

}
