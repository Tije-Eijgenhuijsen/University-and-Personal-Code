package repls

import scala.annotation.tailrec
import scala.collection.mutable
//import scala.collection.mutable.Stack

class IntREPL extends REPLBase {
    // Have a REPL of type Int
    type Base = Int
    override val replName: String = "ints" // TODO: name me!

    var bindings : mutable.Map[String,Int] = mutable.Map[String,Int]()

    val operatorPrecedence : String => Int = {
        case "+" => 2
        case "-" => 2
        case "*" => 3
        case "(" => 0
    }

    abstract class Expression{
        def value : Int

        def describe : String = "The value of " + toString + " is " + value.toString
    }

    case class Constant(n : Int) extends Expression{
        def value : Int = n

        override def toString : String = n.toString
    }

    case class Negate(arg : Expression) extends Expression{
        def value : Int = -arg.value

        override def toString : String = (-arg.value).toString

    }

    case class Var(x : String) extends Expression{
        def value : Int = bindings(x)

        override def toString : String = x
    }

    case class Operator(operatorName : String, lhs : Expression, rhs : Expression, parenthesesNeeded : Boolean = true) extends Expression{
        def value : Int = PatternMatch.operatorByName(operatorName, lhs.value, rhs.value)

        override def toString : String = {
            if(parenthesesNeeded) { "( " + lhs.toString + " " + operatorName + " " + rhs.toString + " )" }
            else{ lhs.toString + " " + operatorName + " " + rhs.toString }
        }

    }

    object PatternMatch {

        def operatorByName(operator : String, lhs : Int, rhs : Int) : Int = {
            operator match{
                case "*" => lhs * rhs
                case "+" => lhs + rhs
                case "-" => lhs - rhs
            }
        }

        def eval(exp: Expression) : Int = {
            exp match{
                case Constant(i) => i
                case Negate(arg) => -eval(arg)
                case Var(x) => bindings(x)
                case Operator(op, lhs, rhs, _) => operatorByName(op, eval(lhs), eval(rhs))
            }
        }

        def simplify(exp : Expression) : Expression = {
            exp match{
                case Var(x) if(bindings.contains(x)) => Constant(bindings(x))
                case Operator("+", Constant(0), e, _) => simplify(e)
                case Operator("+", e, Constant(0), _) => simplify(e)
                case Operator("*", Constant(1), e, _) => simplify(e)
                case Operator("*", e, Constant(1), _) => simplify(e)
                case Operator("*", Constant(0), e, _) => Constant(0)
                case Operator("*", e, Constant(0), _) => Constant(0)
                case Operator("-", l, r, _) if(l == r) => Constant(0)
                case Operator("+",
                        Operator("*",a,b, _),
                        Operator("*",c,d, _), _)
                            if(a == c) =>
                Operator("*", a, Operator("+",b,d ))

                case Operator("+",
                Operator("*",b,a, _),
                Operator("*",c,d, _), _)
                    if(a == c) =>
                    Operator("*", a, Operator("+",b,d ))

                case Operator("+",
                Operator("*",a,b, _),
                Operator("*",d,c, _), _)
                    if(a == c) =>
                    Operator("*", a, Operator("+",b,d ))

                case Operator("+",
                Operator("*",b,a, _),
                Operator("*",d,c, _), _)
                    if(a == c) =>
                    Operator("*", a, Operator("+",b,d ))
                case Operator(op, Constant(lhs), Constant(rhs), _) => Constant(eval(exp))
                case Operator(op, lhs, rhs, _) => Operator(op,simplify(lhs),simplify(rhs))

                case _ => exp
            }
        }
    }


    def toPostfix(input : Array[String]) : String = {
        val saveOperators : mutable.Stack[String] = new mutable.Stack()
        var postfixNotation : String = ""

        def checkPrecedence(curElement : String): Unit = {
            if(saveOperators.isEmpty) {
                saveOperators.push(curElement)
            }else if(saveOperators.top == "("){
                saveOperators.push(curElement)
            }else{
                while(saveOperators.nonEmpty && operatorPrecedence(saveOperators.top) >= operatorPrecedence(curElement)){
                    postfixNotation += saveOperators.pop() + " "
                }
                saveOperators.push(curElement)
            }
        }

        for(el <- input){
            el match{
                case "+" => checkPrecedence(el)
                case "-" => checkPrecedence(el)
                case "*" => checkPrecedence(el)
                case "(" => saveOperators.push(el)
                case ")" => {
                    while (saveOperators.nonEmpty && saveOperators.top != "(") {
                        postfixNotation += saveOperators.pop() + " "
                    }
                    if(saveOperators.nonEmpty) saveOperators.pop()
                }
                case _ => postfixNotation += el + " "
            }
        }
        while(saveOperators.nonEmpty){
            if(saveOperators.top == "("){
                saveOperators.pop()
            }else {
                postfixNotation += saveOperators.pop() + " "
            }
        }
        postfixNotation.trim
    }

    def isOperator(element : String) : Boolean = {
        element match{
            case "*" => true
            case "+" => true
            case "-" => true
            case _ => false
        }
    }

    def isNumber(element : String) : Boolean = {
        if(element.isEmpty) return false
        for(el <- element.toCharArray){
            if(!el.isDigit) return false
        }
        true
    }

    def isLetter(element : String) : Boolean = {
        if(element.isEmpty) return false
        for(el <- element.toCharArray){
            if(!el.isLetter) return false
        }
        true
    }

    def RPNToExpressionTree(expression : String, isSimplification : Boolean) : Expression = {
        val s : mutable.Stack[Expression] = new mutable.Stack()
        for (el <- expression.split(" ")){
            if(isOperator(el)){
                val rhs = s.pop()
                val lhs = s.pop()
                val res = Operator(el, lhs,rhs)
                s.push(res)
            }else if(isNumber(el)){
                s.push(Constant(el.toInt))
            }else if(el.toCharArray.apply(0) == '-' && isNumber(el.toCharArray.drop(1).mkString)){
                s.push(Negate(Constant(el.toCharArray.drop(1).mkString.toInt)))
            }else if(isLetter(el)){
                if(isSimplification || bindings.contains(el)){
                    s.push(Var(el))
                }
            }else{
            }
        }
        s.top
    }

    def prettyPrint(exp : Expression, precedenceOfOuterOp : Int) : Expression = {
        exp match{
            case Operator(op, lhs, rhs, true) => {
                if(operatorPrecedence(op) >= precedenceOfOuterOp){
                    Operator(op, prettyPrint(lhs, operatorPrecedence(op)), prettyPrint(rhs, operatorPrecedence(op)), false)
                }else{
                    Operator(op, prettyPrint(lhs, operatorPrecedence(op)), prettyPrint(rhs, operatorPrecedence(op)))
                }
            }
            case Operator(op, lhs, rhs, false) => Operator(op, prettyPrint(lhs, operatorPrecedence(op)), prettyPrint(rhs, operatorPrecedence(op)))
            case _ => exp
        }


    }

    def firstElementOf(exp: String) : String = exp.split(" ").apply(0)

    def isVarAssignment(elements : Array[String]) : Boolean = elements.contains("=")

    def isSimplification(exp : String) = firstElementOf(exp) == "@"

    override def readEval(command: String): String = {
        val elements = command.split("\\s") // split string based on whitespace
        // TODO: complete me!
        val postfixExpression: String = toPostfix(elements)
        if(isSimplification(postfixExpression)) {
            var expressionTree : Expression = RPNToExpressionTree(postfixExpression, true)
            var fixPoint : Expression = Constant(0)
            while(expressionTree != fixPoint) {
                fixPoint = expressionTree
                expressionTree = PatternMatch.simplify(expressionTree)
            }
            prettyPrint(expressionTree, 0).toString
        }else if(isVarAssignment(elements)){
            var splitExp = postfixExpression.split(" = ")
            bindings += (splitExp(0) -> PatternMatch.eval(RPNToExpressionTree(splitExp(1), false)))
            return splitExp(0) + " = " +  bindings(splitExp(0)).toString
        }else{
            PatternMatch.eval(RPNToExpressionTree(postfixExpression, false)).toString
        }
    }

    // TODO: Implement any further functions that are specifically for an IntREPL
}
