document.addEventListener("DOMContentLoaded", function () {
    Papa.parse("equipos.csv", {
        download: true,
        header: true,
        complete: function (results) {
            console.log("CSV procesado.");
            const data = results.data;
            console.log(data);

            const uniqueTeams = {};


            data.forEach(row => {
                if (!uniqueTeams[row.ID]) {
                    uniqueTeams[row.ID] = {
                        teamName: row['Nombre del Equipo'],
                        products: new Set(),
                        members: []
                    };
                }

                row.Productos.split('|').forEach(product => {
                    uniqueTeams[row.ID].products.add(product);
                });

                // Almacena la información del miembro del equipo
                uniqueTeams[row.ID].members.push({
                    name: row['Nombre Miembro'],
                    lastname: row['Apellido'],
                    specialty: row['Rol'],
                    ci: row['CI'],
                    contact: row['Contacto']
                });
            });

            console.log(uniqueTeams);

            const container = document.getElementById('cards-container');
            Object.values(uniqueTeams).forEach(team => {
                const card = document.createElement('div');
                card.className = 'card';

                card.innerHTML = `
                    <img src="https://img.freepik.com/foto-gratis/equipo-negocios-exitoso-feliz_53876-74892.jpg" alt="Equipo">
                    <H3 class="titulo_tarjeta">${team.teamName}</H3>
                    <p>Productos involucrados:<br>${[...team.products].join(', ')}</p>
                `;


                // Añadir evento de click para abrir el modal
                card.addEventListener('click', function () {
                    document.getElementById('modal_team_name').textContent = `Equipo: ${team.teamName}`;
                    document.getElementById('modal_team_products').textContent = `Productos: ${[...team.products].join(', ')}`;
                    
                     // Contenido de los miembros
                    const membersList = team.members.map(member => 
                        `<li>${member.name} - ${member.specialty}</li>` // Añade más detalles si es necesario
                    ).join('');

                    document.getElementById('modal_team_members').innerHTML = `<ul>${membersList}</ul>`;

                    new bootstrap.Modal(document.getElementById('teamInfoModal')).show();
                });

                container.appendChild(card);
            });


            console.log("Tarjetas generadas.");
        },
        error: function (error) {
            console.error("Error al procesar el CSV:", error);
        }
    });

    Papa.parse("productos.csv", {
        download: true,
        header: true,
        complete: function (results) {
            console.log("CSV de productos procesado.");
            const data = results.data;
            console.log(data);
    
            const container = document.getElementById('cards-container-productos');
            data.forEach(product => {
                if (product.Estado.toLowerCase() === 'activo') {
                    const card = document.createElement('div');
                    card.className = 'card';
    
                    card.innerHTML = `
                        <img src="https://picsum.photos/id/180/200/300" alt="Equipo">
                        <H3 class="titulo_tarjeta">${product.Nombre}</H3>
                        <p>Lenguaje: ${product.Lenguaje}</p>
                        <p>Version: ${product.Version}</p>
                    `;
    
                    // Añadir evento de click para abrir el modal
                    card.addEventListener('click', function () {
                        document.getElementById('modal_product_name').textContent = `Producto: ${product.Nombre}`;
                        document.getElementById('modal_product_description').textContent = `Descripción: ${product.Descripcion}`;
                        document.getElementById('modal_product_version').textContent = `Versión: ${product.Version}`;
                        document.getElementById('modal_product_corporation').textContent = `Corporación: ${product.Corporacion}`;
                        document.getElementById('modal_product_company').textContent = `Empresa: ${product.NombreEmpresa}`;
                        document.getElementById('modal_product_contact').textContent = `Contacto: ${product.Contacto}`;
                        document.getElementById('modal_product_ruc').textContent = `RUC: ${product.RUC}`;
    
                        new bootstrap.Modal(document.getElementById('productInfoModal')).show();
                    });
    
                    container.appendChild(card);
                }
            });
    
            console.log("Tarjetas de productos generadas.");
        },
        error: function (error) {
            console.error("Error al procesar el CSV de productos:", error);
        }
    });

});
