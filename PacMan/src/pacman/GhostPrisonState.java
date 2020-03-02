package pacman;

import java.util.LinkedList;


public class GhostPrisonState extends GhostState {
	private static final int TIMEOUT = 25;


	private LinkedList<Coordinate> path = new LinkedList<Coordinate>();
	
	public GhostPrisonState(Ghost ghost) {
		super(ghost);
		findPath(Ghost.getGateIn());
		timer = TIMEOUT;
	}

	@Override
	protected State getState() {
		return State.PRISON;
	}

	@Override
	protected void move() {
		if(!path.isEmpty())
		{
			ghost.setPosition(path.pop());
		}
		else if(timer != 0)
		{
			ghost.newPosition();
			timerDecrease();
			if(timer == 0)
				findPath(Ghost.getGateOut());
		}
		else 
		{
			ghost.changeState(GhostStateFactory.createGhost(State.NORMAL, ghost));	
		}
	}
	
	private void findPath(Coordinate destination) {
			path = Pathfinding.getPath(ghost.getPosition(), destination, ghost.getGame());
	}

	@Override
	protected void pacmanCollision(Pacman pacman) {

	}

}
