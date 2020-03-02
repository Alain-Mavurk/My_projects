package pacman;



public class Pacman extends Character{
	
	private int life;
	private boolean alive;
	private Direction memoryDirection;
	private PacmanState state;
	
	public Pacman(Game game, Coordinate start) {
		super(game, start);
		setDirection(Direction.Down);
		memoryDirection = Direction.Down;
		life = 3;
		alive = true;
		state = PacmanStateFactory.createPacman(PacmanState.State.NORMAL, this);
	}
	
	public PacmanState.State getPacmanState() {
		return state.getState();
	}
	
	public void initPosition() {
		setPosition(initPosition);
		setDirection(Direction.Down);
		setMemoryDirection(Direction.Down);
	}

	
	public int getLife() {
		return life;
	}
	
	
	public Coordinate nextPosition(Direction direction) {
		return new Coordinate(position.getX()+direction.getX(), position.getY()+direction.getY());
	}

	
	public Direction getMemoryDirection() {
		return memoryDirection;
	}
	
	public void setMemoryDirection(Direction direction) {
		this.memoryDirection = direction;
	}
	
	public void  incLife() {
		life++;
	}
	
	public void lostLife() {
		life--;
	}
	
	public void die() {
		alive = false;
	}
	
	public void checkLife() {
		if(life == 0) {
			die();
		}
	}
	
	public boolean isAlive() {
		return alive;
	}
	
	
	public void changeState(PacmanState state) {
		this.state = state;
	}

	public void ghostCollision(Ghost ghost) {
		state.ghostCollision(ghost);
	}
	
	public void checkPacdotCollision() {
		game.checkPacdotCollision(position);
	}
	
	public void eat(Pacdot pacdot) {
		pacdot.eat();
	}
	
	public void wraparound() {
		if(getGame().isOut(nextPosition()))
			getGame().wraparound(nextPosition());
	}
	
	public void newPosition(CheckCollision checker) {
		if(checker.tester(this)){
			checkPacdotCollision();
			 if(!getGame().wallAndDoorCollision(nextPosition()))
				setPosition(nextPosition());
		}
	}
	
	public void move() {
		state.move();
		autoDirection();
		notifyObserver();
	}

	public void autoDirection() {
		if(!game.wallAndDoorCollision(nextPosition(memoryDirection))) {
			setDirection(memoryDirection);
		}
	}
	
}
