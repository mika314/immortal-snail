#!/bin/bash
scp HTML5/ImmortalSnail.css HTML5/ImmortalSnail.data HTML5/ImmortalSnail.data.js HTML5/ImmortalSnail.js HTML5/ImmortalSnail.UE4.js HTML5/ImmortalSnail.wasm HTML5/ImmortalSnail.worker.js games.mika.global:/var/www/games.mika.global/html/ImmortalSnail/
fname=/usr/share/sounds/sound-icons/glass-water-1.wav
if [ $# != 0 ]; then
	fname=/usr/share/sounds/sound-icons/piano-3.wav
fi
play -q $fname
play -q $fname
play -q $fname
