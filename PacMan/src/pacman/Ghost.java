package pacman;

import java.util.Random;



public class Ghost extends Character{
	private final static Coordinate GATE_OUT = new Coordinate(14,11); 
	private final static Coordinate GATE_IN = new Coordinate(14,14);
	
	private GhostState state;
	
	public Ghost(Game game, Coordinate start) {
		super(game, start);
		setDirection(Direction.Up);
		state = GhostStateFactory.createGhost(GhostState.State.PRISON, this);
	}
	
	
	
	
	public static Coordinate getGateOut() {
		return GATE_OUT;
	}
	
	
	

	public static Coordinate getGateIn() {
		return GATE_IN;
	}
	
	
	

	public Coordinate getInitPosition() {
		return initPosition;
	}
	
	
	
	
	public void generateNewDirection() {
		Random r = new Random();
		Direction d = direction;
		do {
			switch(r.nextInt(4)) {
				case 0: 
					setDirection(Direction.Down);
					break;
				case 1: 
					setDirection(Direction.Up);
					break;
				case 2: 
					setDirection(Direction.Right);
					break;
				default:
					setDirection(Direction.Left);
			}
		}while(d == direction);
	}
	
	public void newPosition() {
		while(getGame().wallAndDoorCollision(nextPosition()))
			generateNewDirection();
		
		game.checkPacmanCollision(this);
		setPosition(nextPosition());
	}
	
	
	
	public void changeState(GhostState state) {
		this.state = state;
	}

	
	
	
	public void move() {
		state.move();
		notifyObserver();
	}
	
	
	
	
	public GhostState.State getState() {
		return state.getState();
	}

	
	
	
	public void pacmanCollision(Pacman pacman) {
		state.pacmanCollision(pacman);
	}
}
