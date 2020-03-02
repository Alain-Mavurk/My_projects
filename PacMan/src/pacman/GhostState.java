package pacman;


public abstract class GhostState {
	
	public enum State{
		NORMAL,
		VULNERABLE,
		PRISON,
	}
	
	Ghost ghost;
	protected int timer;
	
	protected GhostState(Ghost ghost) {
		this.ghost = ghost;
	}
	
	protected void timerDecrease() {
    	timer--;
    }
	
	protected abstract State getState();
	protected abstract void move();
	protected abstract void pacmanCollision(Pacman pacman);
}
