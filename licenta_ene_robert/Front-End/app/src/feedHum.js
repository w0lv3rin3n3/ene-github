function liveUpdateHum() {
  // const feedHum = document.querySelector("#feedHum");
  const textHumData = document.getElementById("HumData");
  setInterval(function firstUpdateHum() {
    fetch("http://localhost:3000/dht11-sensor")
      .then((response) => response.json())
      .then((data) => {
        const ctx = document.getElementById("myChartHum").getContext("2d");
        const dateTimeArr = [];
        for (let element of data) {
          // moment(element.date).format('HH:mm')
          dateTimeArr.push(element.date);
        }
        const valuesArr = [];
        for (let element of data) {
          if (element.sensor_name == "dht11_sensor")
            valuesArr.push(element.value2);
        }
        const myChartHum = new Chart(ctx, {
          type: "line",
          data: {
            //array of strings (datetimes)
            labels: dateTimeArr,
            scaleShowLabels: false,
            datasets: [
              {
                label: "Humidity",
                //array of datetime values
                data: valuesArr,
                backgroundColor: ["rgba(200, 200, 10, 0.3)"],
                borderColor: ["rgba(200, 200, 50, 1)"],
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
              yAxes: [{
                ticks: {
                  suggestedMin: 0,
                  suggestedMax: 70,
                  beginAtZero: true,
                }
              },
            ]
            },
          },
        });
        textHumData.textContent =
        data[data.length - 1].value2 + '%';
      })
      .catch((err) => console.log(err));
      return firstUpdateHum;
  }(), 15000);
}

document.addEventListener("DOMContentLoaded", function () {
  liveUpdateHum();
});