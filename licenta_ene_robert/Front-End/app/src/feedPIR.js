function liveUpdatePIR () {
  const feedPIR = document.querySelector("#feedPIR");
  const textPIRData = document.getElementById('PIRData');
  setInterval(function firstUpdatePIR() {
    fetch("http://localhost:3000/PIR-sensor")
    .then((response) => response.json())
    .then((data) => {
      textPIRData.textContent = 'Motion: ' + data[data.length-1].value1 + ' LEDState: ' + data[data.length-1].value2;
      // feedPIR.insertAdjacentHTML(
      //   "afterbegin",
      //   'Motion: ' + data[data.length-1].value1 + ' LEDState: ' + data[data.length-1].value2
      // );
    })
    .catch((err) => console.log(err));
    return firstUpdatePIR;
  }(), 15000);
}

document.addEventListener('DOMContentLoaded', function () {
  liveUpdatePIR();
})