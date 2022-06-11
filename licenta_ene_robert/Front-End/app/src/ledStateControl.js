const feedDisplay = document.querySelector("#ledState");
var obj = {}
obj.sendValue =  {sendValue: function(state) {
  fetch(`http://localhost:3000/slider-input/send/${state}`)
    .then((response) => response.json())
    .then((data) => {
      feedDisplay.insertAdjacentHTML("afterbegin", "Data sent");
    })
    .catch((err) => console.log(err));
  }
}
