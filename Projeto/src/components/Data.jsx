import React, { useState, useEffect } from 'react';

function Data() {
    const [currentTime, setCurrentTime] = useState(new Date());

    useEffect(() => {
        const intervalId = setInterval(() => {
            setCurrentTime(new Date());
        }, 1000); // Atualiza a cada segundo

        // Limpa o intervalo ao desmontar o componente
        return () => clearInterval(intervalId);
    }, []); // Executa apenas uma vez no início
    const meses = ["Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"];
    const dias_da_semana = ["Domingo","Segunda Feira","Terça Feira","Quarta Feira","Quinta Feira","Sexta Feira","Sabado"];
    const currentDate = new Date();
    const hora = currentDate.getHours();
    const min = currentDate.getMinutes();
    const seg = currentDate.getSeconds();
    const dia_da_semana = currentDate.getDay();
    const dia_do_mes = currentDate.getUTCDate();
    const mes = currentDate.getUTCMonth();

    return (
        <div className='data'>
            <p>{dia_do_mes} {meses[mes]} - {dias_da_semana[dia_da_semana]} - {hora}H{min}:{seg}</p>
        </div>
    );
}

export default Data;
