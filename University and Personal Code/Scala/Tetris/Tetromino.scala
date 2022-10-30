package tetris.logic

abstract class Tetromino(val absoluteBlockPos : Vector[Point],
                         val shape : Vector[Point],
                         val typeOfCell : CellType) {
  protected def addPoints(absolutePos : Point, shapePos : Point) : Point ={
    Point(absolutePos.x + shapePos.x, absolutePos.y + shapePos.y)
  }
  protected def rotateBlock(anchorPos : Point, newRotation : Vector[Point]) : Vector[Point] ={
    Vector[Point](addPoints(anchorPos, newRotation.apply(0)),
                  addPoints(anchorPos, newRotation.apply(1)),
                  addPoints(anchorPos, newRotation.apply(2)),
                  addPoints(anchorPos, newRotation.apply(3)))
  }
  def rotateShapeLeft() : Vector[Point]
  def rotateShapeRight() : Vector[Point]
  def rotateLeft()  : Tetromino
  def rotateRight()  : Tetromino
  def getBlockPos : Vector[Point] = absoluteBlockPos
  def getBlockType : CellType = typeOfCell
  def insideArena(arenaWidth : Int, arenaHeight : Int) : Boolean ={
    for(el <- absoluteBlockPos){
      if(el.x >= arenaWidth || el.x < 0){
        return false
      }else if(el.y >= arenaHeight || el.y < 0){
        return false
      }
    }
    return true
  }
}

abstract class rotatesFromAnchor(override val absoluteBlockPos: Vector[Point],
                                 override val shape: Vector[Point],
                                 override val typeOfCell: CellType)
                                 extends Tetromino(absoluteBlockPos, shape, typeOfCell){

  val rotateCounterClockWise : Point => Point = (pos : Point) => { Point(pos.y, -pos.x) }
  val rotateClockWise : Point => Point = (pos : Point) => { Point(-pos.y, pos.x) }

  def rotateShapeLeft() : Vector[Point] = { shape.map(rotateCounterClockWise) }
  def rotateShapeRight() : Vector[Point]  = { shape.map(rotateClockWise) }

  def rotateLeft() : Tetromino ={
    this match {
      case block: JBlock => new JBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeLeft()), rotateShapeLeft())
      case block: LBlock => new LBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeLeft()), rotateShapeLeft())
      case block: SBlock => new SBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeLeft()), rotateShapeLeft())
      case block: TBlock => new TBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeLeft()), rotateShapeLeft())
      case block: ZBlock => new ZBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeLeft()), rotateShapeLeft())
    }
  }

  def rotateRight() : Tetromino ={
    this match {
      case block: JBlock => new JBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeRight()), rotateShapeRight())
      case block: LBlock => new LBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeRight()), rotateShapeRight())
      case block: SBlock => new SBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeRight()), rotateShapeRight())
      case block: TBlock => new TBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeRight()), rotateShapeRight())
      case block: ZBlock => new ZBlock(rotateBlock(this.absoluteBlockPos.apply(0), rotateShapeRight()), rotateShapeRight())
    }
  }
}

class JBlock(override val absoluteBlockPos: Vector[Point],
             override val shape: Vector[Point] = TetrominoShapes.J,
             override val typeOfCell: CellType = JCell)
             extends rotatesFromAnchor(absoluteBlockPos,shape, typeOfCell)

class LBlock(override val absoluteBlockPos: Vector[Point],
             override val shape: Vector[Point] = TetrominoShapes.L,
             override val typeOfCell: CellType = LCell)
             extends rotatesFromAnchor(absoluteBlockPos, shape, typeOfCell)

class SBlock(override val absoluteBlockPos: Vector[Point],
             override val shape: Vector[Point] = TetrominoShapes.S,
             override val typeOfCell: CellType = SCell)
             extends rotatesFromAnchor(absoluteBlockPos, shape, typeOfCell)

class TBlock(override val absoluteBlockPos: Vector[Point],
             override val shape: Vector[Point] = TetrominoShapes.T,
             override val typeOfCell: CellType = TCell)
             extends rotatesFromAnchor(absoluteBlockPos, shape, typeOfCell)

class ZBlock(override val absoluteBlockPos: Vector[Point],
             override val shape: Vector[Point] = TetrominoShapes.Z,
             override val typeOfCell: CellType = ZCell)
             extends rotatesFromAnchor(absoluteBlockPos, shape, typeOfCell)

class IBlock (override val absoluteBlockPos: Vector[Point],
              override val shape: Vector[Point] = TetrominoShapes.I,
              override val typeOfCell: CellType = ICell)
              extends Tetromino(absoluteBlockPos, shape, typeOfCell){

  val anchorPos : Point = Point(absoluteBlockPos.apply(0).x - shape.apply(0).x, absoluteBlockPos.apply(0).y - shape.apply(0).y)

  val rotateCounterClockWise : Point => Point = (pos : Point) => { Point(pos.y, -pos.x + 1) }
  val rotateClockWise : Point => Point = (pos : Point) => { Point(-pos.y + 1, pos.x) }

  def rotateShapeLeft() : Vector[Point] = { shape.map(rotateCounterClockWise) }
  def rotateShapeRight() : Vector[Point]  = { shape.map(rotateClockWise) }

  def rotateLeft() : Tetromino ={ new IBlock(rotateBlock(anchorPos, rotateShapeLeft()), rotateShapeLeft()) }
  def rotateRight() : Tetromino ={ new IBlock(rotateBlock(anchorPos, rotateShapeRight()), rotateShapeRight()) }
}

class OBlock (override val absoluteBlockPos: Vector[Point],
              override val shape: Vector[Point] = TetrominoShapes.O,
              override val typeOfCell: CellType = OCell)
              extends Tetromino(absoluteBlockPos, shape, typeOfCell){

  def rotateShapeLeft() : Vector[Point] = { shape }
  def rotateShapeRight() : Vector[Point]  = { shape }

  def rotateLeft() : Tetromino = { this }
  def rotateRight() : Tetromino = { this }
}

object TetrominoShapes{
  def I : Vector[Point] = Vector[Point](Point(0,0),Point(-1,0),Point(1,0),Point(2,0))
  def J : Vector[Point] = Vector[Point](Point(0,0),Point(-1,-1),Point(-1,0),Point(1,0))
  def L : Vector[Point] = Vector[Point](Point(0,0),Point(1,-1),Point(-1,0),Point(1,0))
  def O : Vector[Point] = Vector[Point](Point(0,0),Point(0,-1),Point(1,-1),Point(1,0))
  def S : Vector[Point] = Vector[Point](Point(0,0),Point(0,-1),Point(1,-1),Point(-1,0))
  def T : Vector[Point] = Vector[Point](Point(0,0),Point(0,-1),Point(-1,0),Point(1,0))
  def Z : Vector[Point] = Vector[Point](Point(0,0),Point(-1,-1),Point(0,-1),Point(1,0))
}
