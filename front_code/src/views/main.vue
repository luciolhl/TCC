<template>
  <div>
    <div class="mb-5">
      <h1>Controle de enxovais</h1>
      <span>
        Utilize a tabela abaixo para consultar a localização de enxovais em setores específicos
      </span>
    </div>
    <v-row>
      <v-col>
        <v-card>
          <v-card-title>
            <v-text-field v-model="search" append-icon="mdi-magnify" label="Search" single-line
              hide-details></v-text-field>
          </v-card-title>
          <v-data-table :loading="loadingTable" :headers="headers" :items="items" :search="search" rows-per-page-text="Itens por página"></v-data-table>
        </v-card>
      </v-col>
    </v-row>
  </div>
</template>

<script>
import http from '../services/http'
export default {
  name: 'PaginaPrincipal',
  async created() {
    await http.get('tags').then(response => {
      console.log(response.data)
      let data = response.data

      data.map(x =>
        this.items.push({
          categoria_id: x.Categoria === 1 ? 'Banho' : 'Cama',
          item: x.Item,
          rfid_id: x.ID,
          local: x.local === 1 ? 'Em uso' : x.local === 2 ? 'Lavanderia': x.local === 3 ? 'Posto de Enfermagem': 'UTI',
      }))
      this.loadingTable = false
      // console.log('CREATED', this.items)
    })
  },
  data() {
    return {
      search: '',
      items: [],
      headers: [
        {
          text: 'RFID(id)', value: 'rfid_id', align: 'center', width: '10'
        },
        { text: 'Categoria', value: 'categoria_id', align: 'center' },
        { text: 'Item', value: 'item', align: 'center' },
        { text: 'Local', value: 'local', align: 'center' },
      ],
      loadingTable: true
    }
  },
  methods: {
 
  }

}
</script>

<style scoped>
span {
  opacity: 0.7;
  font-style: italic;
}
</style>