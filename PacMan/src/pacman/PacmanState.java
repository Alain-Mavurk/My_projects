package pacman;


public abstract class PacmanState {
	public enum State{
		NORMAL,
		INVISIBLE,
		SUPERPACMAN
	}
	protected Pacman pacman;
	protected int timer;
	
	protected PacmanState(Pacman pacman) {
		this.pacman = pacman;	
	}
	
	protected void timerDecrease() {
    	timer--;
    }
	
	protected void checkTimeout(State state) {
		if(timer == 0)
			pacman.changeState(PacmanStateFactory.createPacman(state, pacman));
    }
	
	protected abstract void move();
	protected abstract State getState();
	protected abstract void ghostCollision(Ghost ghost);
	
}
