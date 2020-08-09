var state = false;
var baseUrl = "https://cute-domain-name.com"

document.addEventListener('DOMContentLoaded', (event) => {
  var heart = document.querySelector('#heart')
  var message = document.querySelector('#message')

  fetch(`${baseUrl}/state`).then(function(response){
    return response.text();
  }).then(function(text) {

    if(text == "on") {
      state = true
    } else if(text == "off") {
      state = false
    }

    if(state) {
      // Make the animation changes     
      heart.classList.remove('smallbeat');
      heart.classList.add('bigbeat');

      message.innerText = "Requesting attention immediately ..."
    } else {
      // Make the animation changes     
      heart.classList.remove('bigbeat');
      heart.classList.add('smallbeat');
      
      message.innerText = "No immediate attention required :)"
    }
  })

  heart.addEventListener('click', function() {
    state = !state

    if(state) {
      // Make the animation changes     
      heart.classList.remove('smallbeat');
      heart.classList.add('bigbeat');

      message.innerText = "Requesting attention immediately ..."

      fetch(`${baseUrl}/on`, {method: 'post'})
    } else {
      // Make the animation changes     
      heart.classList.remove('bigbeat');
      heart.classList.add('smallbeat');
      
      message.innerText = "No immediate attention required :)"
      
      fetch(`${baseUrl}/off`, {method: 'post'})
    }
  })
})

