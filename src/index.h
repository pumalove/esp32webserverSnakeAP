const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
body {
  text-align:center;
  font-family: helvetica;
}
canvas {
  border: 2px solid rgb(151, 149, 149);
}

.hideThis {
    display:none;
}

.button {
	cursor: pointer;
	box-shadow: 0 0 3px 1px #555;
	color: #474881;
	text-align: center;
	
}
.up-btn {
	left: 1em;
	top: 4em;
	border-radius: .25em .25em 0 0;
}
.right-btn {
	top: 4em;
	left: 4em;
	border-radius: 0 .25em .25em 0;
}
.down-btn {
	left: 1em;
	bottom: 2em;
	border-radius: 0 0 .25em .25em;
}
.left-btn {
	top: 1em;
	right: 2em;
	border-radius: .25em 0 0 .25em;
}

.d {
	width: 3em;
	height: 3em;
	background: #1d2323;
	position: relative;
	border-top: .05em solid lighten(#1d2323, 40%);
	border-bottom: .05em solid darken(#1d2323, 20%);
}


</style>
<body>
<h1>Simple Snake Game</h1>
<h3>Score 10 to open the box</h3>
<canvas id="stage" height="400" width="520"></canvas><br>
<a class="unlockBtn hideThis" href="H">Open Geocache</a>
<base href="192.168.4.1/index.html/" />

<div class="d-pad">
    <div class="btn-up">
        <button id="btn-up" class="button d up-btn"> &#9650;</button>
    </div>
    
    <div class="btn-right">
        <button id="btn-right" class="button d right-btn"> &#9654;</button>
    </div>
        
    <div class="btn-left">
        <button id="btn-left" class="button d left-btn"> &#9664;</button>						
    </div>
    
    <div class="btn-down">
        <button id="btn-down" class="button d down-btn"> &#9660;</button>			
    </div>

</div>

<script>
/**
 * Namespace
 */
var Game      = Game      || {};
var Keyboard  = Keyboard  || {}; 
var Component = Component || {};

/**
 * Keyboard Map
 */
Keyboard.Keymap = {
  37: 'left',
  38: 'up',
  39: 'right',
  40: 'down'
};

function showUnlockButton(){
    var unlockButton = document.getElementsByClassName('unlockBtn hideThis');
    while(unlockButton.length > 0){
        unlockButton[0].classList.remove('hideThis');
        }
}



var leftBtn = document.getElementById("btn-left");
leftBtn.addEventListener("click",function(e){
    var evt = new KeyboardEvent('keydown', {'keyCode':37, 'which':37});
    document.dispatchEvent (evt);
},false);

var upBtn = document.getElementById("btn-up");
upBtn.addEventListener("click",function(e){
    var evt = new KeyboardEvent('keydown', {'keyCode':38, 'which':38});
    document.dispatchEvent (evt);
},false);

var rightBtn = document.getElementById("btn-right");
rightBtn.addEventListener("click",function(e){
    var evt = new KeyboardEvent('keydown', {'keyCode':39, 'which':39});
    document.dispatchEvent (evt);
},false);

var downBtn = document.getElementById("btn-down");
downBtn.addEventListener("click",function(e){
    var evt = new KeyboardEvent('keydown', {'keyCode':40, 'which':40});
    document.dispatchEvent (evt);
},false);

/**
 * Keyboard Events
 */
Keyboard.ControllerEvents = function() {
  
  // Setts
  var self      = this;
  this.pressKey = null;
  this.keymap   = Keyboard.Keymap;
  
  // Keydown Event
  document.onkeydown = function(event) {
    self.pressKey = event.which;
  };
  
  // Get Key
  this.getKey = function() {
    return this.keymap[this.pressKey];
  };
};

/**
 * Game Component Stage
 */
Component.Stage = function(canvas, conf) {  
  
  // Sets
  this.keyEvent  = new Keyboard.ControllerEvents();
  this.width     = canvas.width;
  this.height    = canvas.height;
  this.length    = [];
  this.food      = {};
  this.score     = 0;
  this.direction = 'right';
  this.conf      = {
    cw   : 10,
    size : 5,
    fps  : 1000
  };
  
  // Merge Conf
  if (typeof conf == 'object') {
    for (var key in conf) {
      if (conf.hasOwnProperty(key)) {
        this.conf[key] = conf[key];
      }
    }
  }
  
};

/**
 * Game Component Snake
 */
Component.Snake = function(canvas, conf) {
  
  // Game Stage
  this.stage = new Component.Stage(canvas, conf);
  
  // Init Snake
  this.initSnake = function() {
    
    // Itaration in Snake Conf Size
    for (var i = 0; i < this.stage.conf.size; i++) {
      
      // Add Snake Cells
      this.stage.length.push({x: i, y:0});
    }
  };
  
  // Call init Snake
  this.initSnake();
  
  // Init Food  
  this.initFood = function() {
    
    // Add food on stage
    this.stage.food = {
      x: Math.round(Math.random() * (this.stage.width - this.stage.conf.cw) / this.stage.conf.cw), 
      y: Math.round(Math.random() * (this.stage.height - this.stage.conf.cw) / this.stage.conf.cw), 
    };
  };
  
  // Init Food
  this.initFood();
  
  // Restart Stage
  this.restart = function() {
    this.stage.length            = [];
    this.stage.food              = {};
    this.stage.score             = 0;
    this.stage.direction         = 'right';
    this.stage.keyEvent.pressKey = null;
    this.initSnake();
    this.initFood();
  };
};

/**
 * Game Draw
 */
Game.Draw = function(context, snake) {
  
  // Draw Stage
  this.drawStage = function() {
    
    // Check Keypress And Set Stage direction
    var keyPress = snake.stage.keyEvent.getKey(); 
    if (typeof(keyPress) != 'undefined') {
      snake.stage.direction = keyPress;
    }
    
    // Draw White Stage
    context.fillStyle = "white";
    context.fillRect(0, 0, snake.stage.width, snake.stage.height);
    
    // Snake Position
    var nx = snake.stage.length[0].x;
    var ny = snake.stage.length[0].y;
    
    // Add position by stage direction
    switch (snake.stage.direction) {
      case 'right':
        nx++;
        break;
      case 'left':
        nx--;
        break;
      case 'up':
        ny--;
        break;
      case 'down':
        ny++;
        break;
    }
    
    // Check Collision
    if (this.collision(nx, ny) == true) {
      snake.restart();
      return;
    }
    
    var gameIsWon = false;

    if (snake.stage.score >= 2 && !gameIsWon) {
        gameIsWon = true;
        var gradient = context.createLinearGradient(0, 0, 520, 0);
        gradient.addColorStop("0"," magenta");
        gradient.addColorStop("0.5", "blue");
        gradient.addColorStop("1.0", "red");
        context.fillStyle = gradient;
        context.font = "42px Verdana";
        context.fillText("Winner!!!", 10, 90);
        showUnlockButton();
      return;
    }
    
    // Logic of Snake food
    if (nx == snake.stage.food.x && ny == snake.stage.food.y) {
      var tail = {x: nx, y: ny};
      snake.stage.score++;
      snake.initFood();
    } else {
      var tail = snake.stage.length.pop();
      tail.x   = nx;
      tail.y   = ny;  
    }
    snake.stage.length.unshift(tail);
    
    // Draw Snake
    for (var i = 0; i < snake.stage.length.length; i++) {
      var cell = snake.stage.length[i];
      this.drawCell(cell.x, cell.y);
    }
    
    // Draw Food
    this.drawCell(snake.stage.food.x, snake.stage.food.y);
    
    // Draw Score
    context.fillText('Score: ' + snake.stage.score, 5, (snake.stage.height - 5));
  };
  
  // Draw Cell
  this.drawCell = function(x, y) {
    context.fillStyle = 'rgb(170, 170, 170)';
    context.beginPath();
    context.arc((x * snake.stage.conf.cw + 6), (y * snake.stage.conf.cw + 6), 4, 0, 2*Math.PI, false);    
    context.fill();
  };
  
  // Check Collision with walls
  this.collision = function(nx, ny) {  
    if (nx == -1 || nx == (snake.stage.width / snake.stage.conf.cw) || ny == -1 || ny == (snake.stage.height / snake.stage.conf.cw)) {
      return true;
    }
    return false;    
  }
};


/**
 * Game Snake
 */
Game.Snake = function(elementId, conf) {
  
  // Sets
  var canvas   = document.getElementById(elementId);
  var context  = canvas.getContext("2d");
  var snake    = new Component.Snake(canvas, conf);
  var gameDraw = new Game.Draw(context, snake);
  
  // Game Interval
  setInterval(function() {gameDraw.drawStage();}, snake.stage.conf.fps);
};


/**
 * Window Load
 */
window.onload = function() {
  var snake = new Game.Snake('stage', {fps: 100, size: 4});
};
</script>

</body>
</html>
<script>

)=====";