const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html>
  <head>
    <style> 
      .slidecontainer { width: 100%; }
      .slider {
        -webkit-appearance: none;
        appearance: none;
        width: 100%;
        height: 25px;
        background: #d3d3d3;
        outline: none;
        opacity: 0.7;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }
      .slider:hover { opacity: 1; }
      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 25px;
        height: 25px;
        background: #04AA6D;
        cursor: pointer;
      }
      .slider::-moz-range-thumb {
        width: 25px;
        height: 25px;
        background: #04AA6D;
        cursor: pointer;
      }
      .arrow-button {
        width: 60px;
        height: 60px;
        margin: 10px;
        font-size: 16px;
        font-weight: bold;
        color: white;
        background-color: #AA1A04;
        border: none;
        cursor: pointer;
        transition: 0.3s;
      }
      .arrow-button:hover {
        background-color: #037a52;
      }
      .arrow-up::after {
        content: "FORWARD";
      }
      .arrow-down::after {
        content: "REVERSE";
      }
      .arrow-left::after {
        content: "LEFT";
      }
      .arrow-right::after {
        content: "RIGHT";
      }
      .arrow-stop::after {
        content: "STOP!";
      }
      .direction-controls {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        position: absolute;
        top: 70%;
        left: 50%;
        transform: translate(-50%, -50%);
      }
      .horizontal-buttons {
        display: flex;
        justify-content: center;
      }
    </style>
  </head>
  <body>

    <h1>Car Control - Lab 4.2</h1>
    <h2>Team 23: Neel Mulay, Brian Fok, Joseph Masica</h2>

    <h2>Direction Control</h2>
    <div class="direction-controls">
      <button class="arrow-button arrow-up" onclick="sendDirection(1)"></button>
      <div class="horizontal-buttons">
        <button class="arrow-button arrow-left" onclick="sendDirection(2)"></button>
        <button class="arrow-button arrow-right" onclick="sendDirection(3)"></button>
      </div>
      <button class="arrow-button arrow-down" onclick="sendDirection(4)"></button>
      <button class="arrow-button arrow-stop" onclick="sendDirection(0)"></button>
    </div>

    <script>
      function sendDirection(direction) {
        console.log("Direction: " + direction);
        fetch("/sendDirection?value=" + direction);
        // Disable the button temporarily
        setTimeout(() => {
          document.activeElement.blur();
        }, 200);
      }

      // Map keyboard keys to directions
      document.addEventListener("keydown", function(event) {
        switch (event.key) {
          case "w":
          case "W":
            sendDirection(1); // Up
            break;
          case "a":
          case "A":
            sendDirection(2); // Left
            break;
          case "d":
          case "D":
            sendDirection(3); // Right
            break;
          case "s":
          case "S":
            sendDirection(4); // Down
            break;
          case " ":
            sendDirection(0); // Stop (spacebar)
            break;
        }
      });
    </script>

  </body>
</html>
)===";
