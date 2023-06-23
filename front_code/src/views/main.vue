<template>
  <div>
    <div class="mb-3">
      <h1>Controle de enxovais</h1>
      <span>
        Utilize a tabela abaixo para consultar a localização de enxovais em setores específicos
      </span>
      <br>
    </div>
    <div class="mb-2">
      <v-btn @click="dialogDeletar = true" color="warning">Deletar TAG</v-btn>
    </div>
    <v-row>
      <v-col>
        <v-card>
          <v-card-title>
            <v-text-field v-model="search" append-icon="mdi-magnify" label="Search" single-line
              hide-details></v-text-field>
          </v-card-title>
          <v-data-table :loading="loadingTable" :headers="headers" :items="items" :search="search"
            rows-per-page-text="Itens por página"></v-data-table>
        </v-card>
      </v-col>
    </v-row>

    <!-- Dialog para deletar TAGS -->
    <v-row justify="center">
      <v-dialog v-model="dialogDeletar" persistent max-width="290">
        <v-card>
          <v-card-title class="text-h5">
            Deletar TAGS
          </v-card-title>
          <v-card-text>
            <div v-if="!loadingData">
              <v-row>
                <v-text-field v-model="idTag" label="Item ID"></v-text-field>
              </v-row>
            </div>
            <div v-else>
              <v-progress-circular indeterminate  model-value="20" :size="35"></v-progress-circular>
            </div>

          </v-card-text>
          <v-card-actions>
            <v-spacer></v-spacer>
            <v-btn color="sucess darken-1" text @click="deleteTags(idTag)">
              Deletar
            </v-btn>
            <v-btn color="error darken-1" text @click="dialogDeletar = false">
              Fechar
            </v-btn>
          </v-card-actions>
        </v-card>
      </v-dialog>
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
          local: x.Local === 1 ? 'Em uso' : x.local === 2 ? 'Lavanderia' : x.local === 3 ? 'Posto de Enfermagem' : 'UTI',
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
      loadingTable: true,
      dialogDeletar: false,
      loadingData: false,
      idTag: ''
    }
  },
  methods: {
    deleteTags(id) {
      this.loadingData = true
      http.delete(`tags/${id}`).then(() => {
        alert('Item deletado com sucesso')
        window.location.reload()
      }).catch((error) => {
        console.log(error)
        alert('Algo deu errado')
      }).finally(() => {
        this.idTag = ''
        this.loadingData = false
      })
    }
  }

}
</script>

<style scoped>
span {
  opacity: 0.7;
  font-style: italic;
}
</style>