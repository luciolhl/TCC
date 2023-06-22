<template>
  <div>
    <div class="mb-5">
      <h1>Categorias de enxoval</h1>
      <span>
        Utilize a tabela abaixo para consultar e inserir categorias de enxoval
      </span>
    </div>
    <br>
    <div class="mb-3">
      <v-btn class="mr-2 mb-2" color="primary" @click="dialog = true">Cadastrar categoria</v-btn>
      <v-btn class="mb-2" color="warning" @click="dialogDeletar = true">Deletar categoria</v-btn>
    </div>
    <v-row>
      <v-col>
        <v-card>
          <v-data-table :loading="loadingTable" :headers="headers" :items="itemsCategoria"
            rows-per-page-text="Itens por página"></v-data-table>
        </v-card>
      </v-col>
    </v-row>

    <!-- Dialog para cadastrar categoria -->
    <v-row justify="center">
      <v-dialog v-model="dialog" persistent max-width="290">
        <v-card>
          <v-card-title class="text-h5">
            Cadastrar categoria
          </v-card-title>
          <v-card-text>
            <div  v-if="!loadingData">
              <v-row justify="center">
                <v-select v-model="selectCategoria" :items="itemsSelectCategoria" label="Categoria"></v-select>
              </v-row>
              <v-row>
                <v-text-field v-model="itemCategoria" label="Item"></v-text-field>
              </v-row>
            </div>
            <div v-else>
              <v-progress-circular indeterminate  model-value="20" :size="35"></v-progress-circular>
            </div>
          </v-card-text>
          <v-card-actions>
            <v-spacer></v-spacer>
            <v-btn color="sucess darken-1" text @click="cadastrarCategoria">
              Cadastrar
            </v-btn>
            <v-btn color="error darken-1" text @click="dialog = false">
              Fechar
            </v-btn>
          </v-card-actions>
        </v-card>
      </v-dialog>
    </v-row>

    <!-- Dialog para deletar categoria -->
    <v-row justify="center">
      <v-dialog v-model="dialogDeletar" persistent max-width="290">
        <v-card>
          <v-card-title class="text-h5">
            Deletar categoria
          </v-card-title>
          <v-card-text>
            <v-row>
              <v-text-field v-model="deleteCategoria" label="Item"></v-text-field>
            </v-row>
          </v-card-text>
          <v-card-actions>
            <v-spacer></v-spacer>
            <v-btn color="sucess darken-1" text @click="deletarCategoria(deleteCategoria)">
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
  name: 'CategoriaRoupas',
  async created() {
    await http.get('categorias').then(response => {
      console.log(response.data)
      let data = response.data

      data.map(x =>
        this.itemsCategoria.push({
          categoria_roupas: x.categorias === 1 ? 'Banho' : 'Cama',
          categorias_id: x.id,
          item: x.item,
        }))
      this.loadingTable = false
    })
  },
  data() {
    return {
      itemsSelectCategoria: ['Banho', 'Cama'],
      itemsCategoria: [],
      loadingTable: true,
      headers: [
        {
          text: 'Id', value: 'categorias_id', align: 'center', width: '10'
        },
        { text: 'Categorias', value: 'categoria_roupas', align: 'center' },
        { text: 'Item', value: 'item', align: 'center' },
      ],
      dialog: false,
      dialogDeletar: false,
      categoria: '',
      item: '',
      selectCategoria: '',
      itemCategoria: '',
      loadingData: false
    }
  },
  methods: {
    cadastrarCategoria() {
      this.loadingData = true 

      if (this.selectCategoria === 'Banho') {
        this.categoria = 1
      } else {
        this.categoria = 2
      }

      console.log(this.categoria, this.itemCategoria)
      http.post('categorias', {
        categorias: this.categoria,
        item: this.itemCategoria
      }).then(response => {
        console.log('Deu certo', response)
        this.loadingData = false
        this.selectCategoria = '',
        this.itemCategoria = ''
        window.location.reload()
      }).catch(() => {
        alert('Deu merda!')
      })
    },
    deletarCategoria(id) {
      http.delete(`categorias/${id}`).then(() => {
        alert('Item deletado com sucesso')
        window.location.reload()
      }).catch(() => {
        alert('Algo deu errado')
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