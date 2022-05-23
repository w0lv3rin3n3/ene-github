const feedDisplay = document.querySelector("#feed");

fetch("http://localhost:3000/2nd-sensor")
  .then((response) => response.json())
  .then((data) => {
    feedDisplay.insertAdjacentHTML(
      "afterbegin",
      "Temperature: " + data.primul + "<br>Humidity: " + data.al_doilea
    );
  })
  .catch((err) => console.log(err));
