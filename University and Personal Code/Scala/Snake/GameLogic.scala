package snake.logic

import engine.random.{RandomGenerator, ScalaRandomGen}
import org.scalacheck.Prop.False
//import snake.game.SnakeGame
import snake.logic.GameLogic._

import scala.collection.mutable.Stack
import scala.collection.immutable.Vector


/** To implement Snake, complete the ``TODOs`` below.
  *
  * If you need additional files,
  * please also put them in the ``snake`` package.
  */
class GameLogic(val random: RandomGenerator,
                val gridDims : Dimensions) {

  class Snake(val headPos : Point = Point(2,0),
              val bodyPos : Vector[Point] = Vector[Point](Point(0,0), Point(1,0)),
              val curDir : Direction = East(),
              val cellsToGrow : Int = 0)

  class GameState(val snake : Snake = new Snake(),
                  val applePos : Point = Point(-1,-1))

  var gameState = new GameState()
  var gameHistory = new Stack[GameState]
  var isReverse : Boolean = false

  if(spaceForApple()){
    updateApple()
  }

  def gameOver: Boolean = gameState.snake.bodyPos.contains(gameState.snake.headPos)
  gameHistory.push(gameState)

  // TODO implement me
  def step(): Unit = {
    if(!isReverse) {
      if (!gameOver) {
        updateBody()
        moveHead()
        if (ateApple()) {
          if (spaceForApple()) {
            updateApple()
          }
          increaseCellsToGrow()
        }
        gameHistory.push(gameState)
      }
    }else{
      reverseStep()
    }

  }

  def reverseStep(): Unit ={
    if(!isStackEmpty()) {
      val previousGameState: GameState = gameHistory.pop()
      gameState = new GameState(previousGameState.snake, previousGameState.applePos)
    }
  }

  def updateBody(): Unit ={
    if (gameState.snake.cellsToGrow == 0) {
      moveBody()
    } else {
      growBody()
    }
  }

  def growBody(): Unit ={ gameState = new GameState(new Snake(gameState.snake.headPos,gameState.snake.bodyPos.appended(gameState.snake.headPos), gameState.snake.curDir, (gameState.snake.cellsToGrow - 1)), gameState.applePos) }

  def moveBody(): Unit ={
    for(i <- 0 until gameState.snake.bodyPos.length - 1){
      gameState = new GameState(new Snake(gameState.snake.headPos, gameState.snake.bodyPos.updated(i, gameState.snake.bodyPos.apply(i + 1)), gameState.snake.curDir, gameState.snake.cellsToGrow), gameState.applePos)
    }
    gameState = new GameState(new Snake(gameState.snake.headPos, gameState.snake.bodyPos.updated((gameState.snake.bodyPos.length - 1), gameState.snake.headPos), gameState.snake.curDir, gameState.snake.cellsToGrow), gameState.applePos)
  }

  def updateApple(): Unit ={ gameState = new GameState(gameState.snake, applePosGen()) }

  def increaseCellsToGrow(): Unit ={ gameState = new GameState(new Snake(gameState.snake.headPos, gameState.snake.bodyPos, gameState.snake.curDir, (gameState.snake.cellsToGrow + 3)), gameState.applePos) }

  def getNewHeadPos(headDirection : Direction = gameState.snake.curDir) : Point = {
    headDirection match{
      case East() => if(gameState.snake.headPos.x + 1 >= gridDims.width){
        return Point(0,gameState.snake.headPos.y)
      }else{
        return Point(gameState.snake.headPos.x + 1,gameState.snake.headPos.y)
      }
      case North() => if(gameState.snake.headPos.y - 1 < 0){
        return Point(gameState.snake.headPos.x,gridDims.height - 1)
      }else{
        return Point(gameState.snake.headPos.x,gameState.snake.headPos.y - 1)
      }
      case West() => if(gameState.snake.headPos.x - 1 < 0){
        return Point(gridDims.width - 1,gameState.snake.headPos.y)
      }else{
        return Point(gameState.snake.headPos.x - 1,gameState.snake.headPos.y)
      }
      case South () => if(gameState.snake.headPos.y + 1 >= gridDims.height){
        return Point(gameState.snake.headPos.x,0)
      }else{
        return Point(gameState.snake.headPos.x,gameState.snake.headPos.y + 1)
      }
    }
  }

  def spaceForApple(): Boolean ={calcNrFreeSpots() > 0}

  def moveHead(): Unit ={ gameState = new GameState(new Snake(getNewHeadPos(), gameState.snake.bodyPos, gameState.snake.curDir, gameState.snake.cellsToGrow), gameState.applePos) }

  def ateApple(): Boolean ={ gameState.snake.headPos == gameState.applePos }

  def calcNrFreeSpots(): Int ={ return (gridDims.height * gridDims.width) - (gameState.snake.bodyPos.length + 1) }

  def isStackEmpty() : Boolean ={ gameHistory.length == 0 }

  def applePosGen() : Point = {
    val randPos : Int = random.randomInt(calcNrFreeSpots())
    var curFreeSpot : Int = -1
    var newApplePos : Point = Point(0,0)

    for (i <- 0 until gridDims.height) {
      for (j <- 0 until gridDims.width) {
        if (!gameState.snake.bodyPos.contains(Point(j, i)) && gameState.snake.headPos != Point(j, i)) {
          curFreeSpot += 1
          if(curFreeSpot == randPos){
            newApplePos = Point(j,i)
          }
        }
      }
    }
    return newApplePos
  }

  // TODO implement me
  def setReverse(r: Boolean): Unit = {
    if(r == true && !isStackEmpty()){
      gameHistory.pop()
    }else if(r == false){
      gameHistory.push(gameState)
    }
    isReverse = r
  }

  // TODO implement me
  def changeDir(d: Direction): Unit = {
    d match {
      case East() => if(gameState.snake.bodyPos.last != getNewHeadPos(East())){
        gameState = new GameState(new Snake(gameState.snake.headPos, gameState.snake.bodyPos, East(), gameState.snake.cellsToGrow), gameState.applePos)
      }
      case North() => if(gameState.snake.bodyPos.last != getNewHeadPos(North())){
        gameState = new GameState(new Snake(gameState.snake.headPos, gameState.snake.bodyPos, North(), gameState.snake.cellsToGrow), gameState.applePos)
      }
      case West() => if(gameState.snake.bodyPos.last != getNewHeadPos(West())){
        gameState = new GameState(new Snake(gameState.snake.headPos, gameState.snake.bodyPos, West(), gameState.snake.cellsToGrow), gameState.applePos)
      }
      case South () => if(gameState.snake.bodyPos.last != getNewHeadPos(South())){
        gameState = new GameState(new Snake(gameState.snake.headPos, gameState.snake.bodyPos, South(), gameState.snake.cellsToGrow), gameState.applePos)
      }
    }
  }

  // TODO implement me
  def getCellType(p : Point): CellType = {
    if(p == gameState.snake.headPos) {
      return SnakeHead(gameState.snake.curDir)
    }else if(gameState.snake.bodyPos.contains(p)) {
      return SnakeBody()
    }else if(p == gameState.applePos){
      return Apple()
    }else{
      return Empty()
    }
  }
}

/** GameLogic companion object */
object GameLogic {

  val FramesPerSecond: Int = 5 // change this to increase/decrease speed of game

  val DrawSizeFactor = 1.0 // increase this to make the game bigger (for high-res screens)
  // or decrease to make game smaller

  // These are the dimensions used when playing the game.
  // When testing the game, other dimensions are passed to
  // the constructor of GameLogic.
  //
  // DO NOT USE the variable DefaultGridDims in your code!
  //
  // Doing so will cause tests which have different dimensions to FAIL!
  //
  // In your code only use gridDims.width and gridDims.height
  // do NOT use DefaultGridDims.width and DefaultGridDims.height
  val DefaultGridDims
  : Dimensions =
  Dimensions(width = 25, height = 25)  // you can adjust these values to play on a different sized board



}


