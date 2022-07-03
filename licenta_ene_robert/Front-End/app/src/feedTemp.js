function liveUpdateTemp() {
  // const feedTemp = document.querySelector("#feedTemp");
  const textTempData = document.getElementById("TempData");
  setInterval(
    (function firstUpdateTemp() {
      fetch("http://localhost:3000/dht11-sensor")
        .then((response) => response.json())
        .then((data) => {
          const ctx = document.getElementById("myChartTemp").getContext("2d");
          const dateTimeArr = [];
          for (let element of data) {
            // moment(element.date).format('HH:mm')
            dateTimeArr.push(element.date);
          }
          const valuesArr = [];
          for (let element of data) {
            if (element.sensor_name == "dht11_sensor")
              valuesArr.push(element.value1);
          }
          if (window.myCharts != undefined) window.myCharts.destroy();
          window.myCharts = new Chart(ctx, {
            type: "line",
            data: {
              //array of strings (datetimes)
              labels: dateTimeArr,
              scaleShowLabels: false,
              datasets: [
                {
                  label: "Temperature",
                  //array of datetime values
                  data: valuesArr,
                  backgroundColor: ["rgba(10, 200, 10, 0.2)"],
                  borderColor: ["rgba(50, 100, 50, 1)"],
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
                yAxes: [
                  {
                    ticks: {
                      suggestedMin: 0,
                      suggestedMax: 70,
                      beginAtZero: true,
                    },
                  },
                ],
              },
            },
          });
          textTempData.textContent = data[data.length - 1].value1 + "°C";
        })
        .catch((err) => console.log(err));
      return firstUpdateTemp;
    })(),
    15000
  );
}

document.addEventListener("DOMContentLoaded", function () {
  liveUpdateTemp();
});
