// Recupera la posizione GPS dal backend
fetch('/gps')
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        const lat = parseFloat(data.latitude);
        const lon = parseFloat(data.longitude);

        // Stampa la posizione GPS nella console
        console.log(`Posizione GPS: Latitudine ${lat}, Longitudine ${lon}`);

        // Crea la mappa centrata sulla posizione GPS
        const map = L.map('map').setView([lat, lon], 13);

        // Aggiungi il layer tile OpenStreetMap
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            attribution: '&copy; OpenStreetMap contributors'
        }).addTo(map);

        // Aggiungi un marker sulla posizione GPS
        L.marker([lat, lon]).addTo(map)
            .bindPopup('La tua posizione attuale').openPopup();
    })
    .catch(error => {
        console.error('Errore nel recupero della posizione GPS:', error);
    });
