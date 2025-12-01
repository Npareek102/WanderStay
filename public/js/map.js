mapboxgl.accessToken = mapToken;

const map = new mapboxgl.Map({
    container: 'map', // container ID
    style: 'mapbox://styles/mapbox/streets-v12',
    center: listing.geometry.coordinates, // Use the global variable defined in HTML
    zoom: 9
});

// Create the marker using the same global variable
const marker = new mapboxgl.Marker({color: "red"})
    .setLngLat(listing.geometry.coordinates) 
    .setPopup(new mapboxgl.Popup({offset: 25})
    .setHTML(`<h4>${listing.location}</h4><p>Exact location will be provided after booking</p>`))
    .addTo(map);