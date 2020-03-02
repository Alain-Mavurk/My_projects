package pacman;



public class GhostVulnerableState extends GhostState {
	private static final int TIMEOUT = 75;
	
	public GhostVulnerableState(Ghost ghost) {
		super(ghost);
		timer = TIMEOUT;
	}


	@Override
	protected void move() {
		if(timer % 2 == 0) {
			ghost.newPosition();
		}
		timerDecrease();
		if(timer == 0)
			ghost.changeState(GhostStateFactory.createGhost(State.NORMAL, ghost));

	}

	@Override
	protected void pacmanCollision(Pacman pacman) {
		ghost.changeState(GhostStateFactory.createGhost(State.PRISON, ghost));
	}

	@Override
	protected State getState() {
		return State.VULNERABLE;
	}

}
