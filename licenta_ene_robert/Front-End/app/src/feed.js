const feedDisplay = document.querySelector("#feed");

fetch("http://localhost:3000/2nd-sensor")
  .then((response) => response.json())
  .then((data) => {
    feedDisplay.insertAdjacentHTML(
      "afterbegin",
      "Temperature: " + data.temperature + "<br>Humidity: " + data.humidity
    );
  })
  .catch((err) => console.log(err));
