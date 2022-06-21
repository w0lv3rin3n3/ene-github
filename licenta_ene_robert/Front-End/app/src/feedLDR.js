const feedLDR = document.querySelector("#feedLDR");
fetch("http://localhost:3000/LDR-sensor")
  .then((response) => response.json())
  .then((data) => {
    feedLDR.insertAdjacentHTML(
      "afterbegin",
      'Luminosity: ' + data[data.length-1].value1 + ' LEDState: ' + data[data.length-1].value2
    );
  })
  .catch((err) => console.log(err));
