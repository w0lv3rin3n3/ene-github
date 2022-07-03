function liveUpdateBTNS () {
    const feedBTN = document.querySelector("#feedBTN");
    const textBTN1Data = document.getElementById('btn1');
    const textBTN2Data = document.getElementById('btn2');
    const textBTN3Data = document.getElementById('btn3');
    const textBTN4Data = document.getElementById('btn4');
    const textBTN5Data = document.getElementById('btn5');
    const textBTN6Data = document.getElementById('btn6');
    const textBTN7Data = document.getElementById('btn7');
    const textBTN8Data = document.getElementById('btn8');
    setInterval(function firstUpdateBTN() {
      fetch("http://localhost:3000/buttons")
      .then((response) => response.json())
      .then((data) => {
        textBTN1Data.textContent = data[data.length-1].btn1name;
        textBTN2Data.textContent = data[data.length-1].btn2name;
        textBTN3Data.textContent = data[data.length-1].btn3name;
        textBTN4Data.textContent = data[data.length-1].btn4name;
        textBTN5Data.textContent = data[data.length-1].btn5name;
        textBTN6Data.textContent = data[data.length-1].btn6name;
        textBTN7Data.textContent = data[data.length-1].btn7name;
        textBTN8Data.textContent = data[data.length-1].btn8name;
      })
      .catch((err) => console.log(err));
      return firstUpdateBTN;
    }(), 15000);
  }
  
  document.addEventListener('DOMContentLoaded', function () {
    liveUpdateBTNS();
  })