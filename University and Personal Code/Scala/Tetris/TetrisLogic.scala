package tetris.logic

import engine.random.{RandomGenerator, ScalaRandomGen}
import tetris.logic.TetrisLogic._

/** To implement Tetris, complete the ``TODOs`` below.
 *
 * If you need additional files,
 * please also put them in the ``tetris`` package.
 */
class TetrisLogic(val randomGen: RandomGenerator,
                  val gridDims : Dimensions,
                  val initialBoard: Seq[Seq[CellType]]) {

  def this(random: RandomGenerator, gridDims : Dimensions) =
    this(random, gridDims, makeEmptyBoard(gridDims))

  def this() =
    this(new ScalaRandomGen(), DefaultDims,    makeEmptyBoard(DefaultDims))

  object Move {
    def down : Point => Point = (pos: Point) => { Point(pos.x, pos.y + 1) }
    def left : Point => Point = (pos: Point) => { Point(pos.x - 1, pos.y) }
    def right : Point => Point = (pos: Point) => { Point(pos.x + 1, pos.y) }
    def toStart : Point => Point = (pos: Point) => { Point(pos.x + anchorStartPos.x, pos.y + anchorStartPos.y) }
  }

  val nrTetrominoes : Int = 7
  val anchorStartPos : Point = Point((gridDims.width/2) - 1 + (gridDims.width % 2), 1)
  var board : Seq[Seq[CellType]] = initialBoard
  var curTetromino : Tetromino = createNewCurTetromino()
  val giveY : Point => Int = (pos: Point) => {pos.y}

  def createNewCurTetromino() : Tetromino = {
    val tetrominoIndex : Int = randomGen.randomInt(nrTetrominoes)
    tetrominoIndex match {
      case 0 => calcTetrominoPos(new IBlock(TetrominoShapes.I), ICell, Move.toStart)
      case 1 => calcTetrominoPos(new JBlock(TetrominoShapes.J), JCell, Move.toStart)
      case 2 => calcTetrominoPos(new LBlock(TetrominoShapes.L), LCell, Move.toStart)
      case 3 => calcTetrominoPos(new OBlock(TetrominoShapes.O), OCell, Move.toStart)
      case 4 => calcTetrominoPos(new SBlock(TetrominoShapes.S), SCell, Move.toStart)
      case 5 => calcTetrominoPos(new TBlock(TetrominoShapes.T), TCell, Move.toStart)
      case 6 => calcTetrominoPos(new ZBlock(TetrominoShapes.Z), ZCell, Move.toStart)
    }
  }

  def calcTetrominoPos(toMove : Tetromino,typeOfTetromino : CellType, moveDirection : Point => Point) : Tetromino = {
    typeOfTetromino match{
      case ICell => new IBlock(toMove.getBlockPos.map[Point](moveDirection),toMove.shape)
      case JCell => new JBlock(toMove.getBlockPos.map[Point](moveDirection),toMove.shape)
      case LCell => new LBlock(toMove.getBlockPos.map[Point](moveDirection),toMove.shape)
      case OCell => new OBlock(toMove.getBlockPos.map[Point](moveDirection),toMove.shape)
      case SCell => new SBlock(toMove.getBlockPos.map[Point](moveDirection),toMove.shape)
      case TCell => new TBlock(toMove.getBlockPos.map[Point](moveDirection),toMove.shape)
      case ZCell => new ZBlock(toMove.getBlockPos.map[Point](moveDirection),toMove.shape)
    }
  }

  def hasDroppedBlock(movedTetPos : Vector[Point]) : Boolean ={
    for(el <- movedTetPos){
      if(board.apply(el.y).apply(el.x) != Empty){
        return true
      }
    }
    return false
  }

  def canMove(movedTet : Tetromino) : Boolean ={
    if(movedTet.insideArena(gridDims.width,gridDims.height) && !hasDroppedBlock(movedTet.getBlockPos)){
      return true
    }
    return false
  }

  def dropBlock(): Unit ={
    updateBoard()
    curTetromino = createNewCurTetromino()
  }

  def checkForFullRow(row : Seq[CellType]): Boolean ={
    for(el <- row){
      if(el == Empty){
        return false
      }
    }
    true
  }

  def updateRows(): Unit ={
    var tempVar = 0
    for(i <- board.indices){
      if(checkForFullRow(board(i))){
        tempVar = i
        while(tempVar > 0){
          board = board.updated(tempVar, board.apply(tempVar - 1))
          tempVar -= 1
        }
      }
    }
  }

  def updateBoard(): Unit ={
    for(el <- curTetromino.getBlockPos){
      board = board.updated(el.y, board.apply(el.y).updated(el.x, curTetromino.getBlockType))
    }
    updateRows()


  }

  def moveTetromino(moveDirection : Point => Point): Unit ={
    val newTetrominoPos = calcTetrominoPos(curTetromino, curTetromino.getBlockType, moveDirection)
    if(canMove(newTetrominoPos)) {
      curTetromino = newTetrominoPos
    }else if(moveDirection == Move.down){
      dropBlock()
    }
  }

  def rotateTetromino(newRotation : Tetromino): Unit ={
    if(canMove(newRotation)) {
      curTetromino = newRotation
    }
  }

  def tryHardDrop(newTetromino : Tetromino) : Tetromino = {
    val oneDown : Tetromino = calcTetrominoPos(newTetromino,newTetromino.getBlockType,Move.down)
    if(canMove(oneDown)) {
      return tryHardDrop(oneDown)
    }
    return newTetromino
  }

  // TODO implement me
  def rotateLeft(): Unit = {
    if(!isGameOver) {
      rotateTetromino(curTetromino.rotateLeft())
    }
  }


  // TODO implement me
  def rotateRight(): Unit = {
    if(!isGameOver) {
      rotateTetromino(curTetromino.rotateRight())
    }
  }

  // TODO implement me
  def moveLeft(): Unit = {
    if(!isGameOver) {
      moveTetromino(Move.left)
    }
  }

  // TODO implement me
  def moveRight(): Unit = {
    if(!isGameOver) {
      moveTetromino(Move.right)
    }
  }

  // TODO implement me
  def moveDown(): Unit = {
    if(!isGameOver) {
      moveTetromino(Move.down)
    }
  }

  // TODO implement me
  def doHardDrop(): Unit = {
    if(!isGameOver) {
      curTetromino = tryHardDrop(curTetromino)
      dropBlock()
    }
  }

  // TODO implement me
  def isGameOver: Boolean = {
    if(curTetromino.getBlockPos.apply(0) == anchorStartPos){
      return !canMove(curTetromino)
    }
    false
  }

  // TODO implement me
  def getCellType(p : Point): CellType = {
    if(curTetromino.getBlockPos.contains(p)){
      curTetromino.getBlockType
    }else{
      board.apply(p.y).apply(p.x)
    }
  }

}

object TetrisLogic {

  val FramesPerSecond: Int = 5 // change this to speed up or slow down the game

  val DrawSizeFactor = 1.0 // increase this to make the game bigger (for high-res screens)
  // or decrease to make game smaller



  def makeEmptyBoard(gridDims : Dimensions): Seq[Seq[CellType]] = {
    val emptyLine = Seq.fill(gridDims.width)(Empty)
    Seq.fill(gridDims.height)(emptyLine)
  }


  // These are the dimensions used when playing the game.
  // When testing the game, other dimensions are passed to
  // the constructor of GameLogic.
  //
  // DO NOT USE the variable DefaultGridDims in your code!
  //
  // Doing so will cause tests which have different dimensions to FAIL!
  //
  // In your code only use gridDims.width and gridDims.height
  // do NOT use DefaultDims.width and DefaultDims.height


  val DefaultWidth: Int = 10
  val NrTopInvisibleLines: Int = 4
  val DefaultVisibleHeight: Int = 20
  val DefaultHeight: Int = DefaultVisibleHeight + NrTopInvisibleLines
  val DefaultDims : Dimensions = Dimensions(width = DefaultWidth, height = DefaultHeight)


  def apply() = new TetrisLogic(new ScalaRandomGen(),
    DefaultDims,
    makeEmptyBoard(DefaultDims))

}