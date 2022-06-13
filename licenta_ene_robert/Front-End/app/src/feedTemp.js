const feedDisplayTemp = document.querySelector("#feedTemp");
fetch("http://localhost:3000/dht11-sensor")
  .then((response) => response.json())
  .then((data) => {
    const ctx = document.getElementById("myChartTemp").getContext("2d");
    const dateTimeArr = [];
    for(let element of data) {
      // moment(element.date).format('HH:mm')
      dateTimeArr.push(element.date);
    }
    const valuesArr = [];
    for(let element of data) {
      if(element.sensor_name == 'temp')
        valuesArr.push(element.arg1);
    }
    const myChartTemp = new Chart(ctx, {
      type: "line",
      data: {
        //array of strings (datetimes)
        labels: dateTimeArr,
        datasets: [
          {
            label: "Temperature",
            //array of datetime values
            data: valuesArr, 
            backgroundColor: [
              "rgba(255, 99, 132, 0.2)",
              "rgba(54, 162, 235, 0.2)",
              "rgba(255, 206, 86, 0.2)",
              "rgba(75, 192, 192, 0.2)",
              "rgba(153, 102, 255, 0.2)",
              "rgba(255, 159, 64, 0.2)",
            ],
            borderColor: [
              "rgba(255, 99, 132, 1)",
              "rgba(54, 162, 235, 1)",
              "rgba(255, 206, 86, 1)",
              "rgba(75, 192, 192, 1)",
              "rgba(153, 102, 255, 1)",
              "rgba(255, 159, 64, 1)",
            ],
            borderWidth: 1,
          },
        ],
      },
      options: {
        responsive: true,
        scales: {
          y: {
            beginAtZero: true,
          },
        },
      },
    });
    feedDisplayTemp.insertAdjacentHTML(
      "afterbegin",
      "Temperature: " + data[data.length-1].arg1 + "<br>Humidity: " + data[data.length-1].arg2 + "<br>Date: " + data[data.length-1].date
    );
  })
  .catch((err) => console.log(err));



//   setTimeout(function(){
//     window.location.reload(1);
//  }, 20000);