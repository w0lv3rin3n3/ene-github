function liveUpdateLDR() {
  // const feedLDR = document.querySelector("#feedLDR");
  const textLDRData = document.getElementById("LDRData");
  setInterval(function firstUpdateLDR() {
    fetch("http://localhost:3000/LDR-sensor")
      .then((response) => response.json())
      .then((data) => {
        textLDRData.textContent =
          "Luminosity: " +
          data[data.length - 1].value1 +
          " LEDState: " +
          data[data.length - 1].value2;
        // feedLDR.insertAdjacentHTML(
        // "afterbegin",
        // 'Luminosity: ' + data[data.length-1].value1 + ' LEDState: ' + data[data.length-1].value2
      })
      .catch((err) => console.log(err));
      return firstUpdateLDR;
    }(), 1000);
}

document.addEventListener("DOMContentLoaded", function () {
  liveUpdateLDR();
});
