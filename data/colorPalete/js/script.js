const colorPicker = document.getElementById('colorPicker');
const colorDisplay = document.getElementById('colorDisplay');
const switchLed = document.getElementById('switchLed');
const rangeBrightnes = document.getElementById('brightness')
colorDisplay.style.backgroundColor = colorPicker.value;

let timeout;

colorPicker.addEventListener('input', async function() {
    colorDisplay.style.backgroundColor = colorPicker.value;
    await post_state(switchLed.checked, colorPicker.value, brightness.value);
});

switchLed.addEventListener('input', async function() {
    await post_state(switchLed.checked, colorPicker.value, brightness.value);
})

rangeBrightnes.addEventListener('input', async function() {
  await post_state(switchLed.checked, colorPicker.value, brightness.value);
})

async function post_color(color) {
    try {
        url="http://192.168.31.200/get_color?color=" + color.substring(1, color.length);
        console.log(url);
        await fetch(url, {
            method: 'GET',
            headers: {
              'Accept': 'application/json',
              'Content-Type': 'application/json'
            },
          });
    } catch (e) {
        console.log(e);
    }
}

async function post_state(state, color, brightness) {
  clearTimeout(timeout);
  timeout = setTimeout(async function() {
    try {
        body = {
          state: state,
          color: color,
          brightness: brightness
        }
        url="http://192.168.31.200/update_state";
        console.log(url);
        console.log(body);
        await fetch(url, {
            method: 'POST',
            headers: {
              'Accept': 'application/json',
              'Content-Type': 'application/json'
            },
            body: JSON.stringify(body)
          });
    } catch (e) {
        console.log(e);
    }
  }, 5);
}

document.addEventListener("DOMContentLoaded", function(){
    console.log("page was loaded");
    fetch('/getCurentState')
      .then(response => response.json())
      .then(data => {
        document.getElementById('colorPicker').value = data.color;
        document.getElementById('brightness').value = data.brightness;
        document.getElementById('switchLed').checked = data.state
        document.getElementById('colorDisplay').style.backgroundColor = data.color;
      })
      .catch(error => console.error('Error fetching data:', error));
    });