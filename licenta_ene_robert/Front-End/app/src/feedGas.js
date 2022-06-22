function liveUpdateGas() {
    // const feedGas = document.querySelector("#feedGas");
    const textGasData = document.getElementById("GasData");
    const textGasWarning = document.getElementById("gasWarning");
    setInterval(function firstUpdateGas() {
      fetch("http://localhost:3000/gas-sensor")
        .then((response) => response.json())
        .then((data) => {
          const ctx = document.getElementById("myChartGas").getContext("2d");
          const dateTimeArr = [];
          for (let element of data) {
            // moment(element.date).format('HH:mm')
            dateTimeArr.push(element.date);
          }
          const valuesArr = [];
          for (let element of data) {
            if (element.sensor_name == "gas_sensor")
              valuesArr.push(element.value1);
          }
          const myChartGas = new Chart(ctx, {
            type: "line",
            data: {
              //array of strings (datetimes)
              labels: dateTimeArr,
              scaleShowLabels: false,
              datasets: [
                {
                  label: "Gas",
                  //array of datetime values
                  data: valuesArr,
                  backgroundColor: [
                    "rgba(150, 10, 10, 0.2)",
                  ],
                  borderColor: [
                    "rgba(200, 50, 50, 1)",
                  ],
                  borderWidth: 2,
                },
              ],
            },
            options: {
              responsive: true,
              scales: {
                xAxes: [
                  {
                    display: false,
                  },
                ],
                y: {
                  beginAtZero: true,
                },
              },
            },
          });
          
          if(data[data.length - 1].value1 > 300)
          {
            textGasData.style.color = "rgb(226, 0, 0)";
            textGasData.textContent = data[data.length - 1].value1 + 'ppm';
            textGasWarning.textContent = "The amount of gas may cause serious damage to health";
            textGasWarning.style.color = "rgb(226, 0, 0)";
          }
          else
          {
            textGasData.textContent = data[data.length - 1].value1 + 'ppm';
            textGasWarning.textContent = "Gas";
          }
        })
        .catch((err) => console.log(err));
        return firstUpdateGas;
    }(), 15000);
  }
  
  document.addEventListener("DOMContentLoaded", function () {
    liveUpdateGas();
  });