package pacman;

import java.util.ArrayList;

public abstract class Character extends Element{
	protected Coordinate initPosition;
	protected Direction direction;
	protected ArrayList<Observer> observers;
	
	protected Character(Game game, Coordinate start) {
		super(game, start);
		initPosition = start;
		observers = new ArrayList<>();
	}
	

	protected void setPosition(Coordinate position) {
		this.position = position;
	}

	
	
	public Coordinate nextPosition() {
		return new Coordinate(position.getX()+direction.getX(), position.getY()+direction.getY());
	}

	
	
	protected Direction getDirection() {
		return direction;
	}



	protected void setDirection(Direction direction) {
		this.direction = direction;
	}

	
	
	protected void addObserver(Observer o) {
		observers.add(o);
	}
	
	
	
	protected void notifyObserver() {
		for (Observer observer : observers) {
			observer.update();
		}
	}

	
	
	protected abstract void move();
}
