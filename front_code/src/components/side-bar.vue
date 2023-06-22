<template>
  <v-navigation-drawer v-model="drawer" fixed app style="background: #ddd;" class="nav" width="250">
    <v-toolbar style="background: #ddd;" class="text-center" height="70px" flat>
      <h1>TCC</h1>
    </v-toolbar>

    <v-list class="text-center mb-2" dense style="background: #666; color: #fff; ">
      <v-list-tile>
        <v-list-tile-content>
          <v-list-tile-title>
            <router-link style="text-decoration: none; color: white;" to="/">Home</router-link>
          </v-list-tile-title>
        </v-list-tile-content>
      </v-list-tile>
    </v-list>

    <v-list class="text-center " dense style="background: #666; color: #fff; ">
      <v-list-tile to="/categoria">
        <v-list-tile-content>
          <v-list-tile-title>
            <router-link style="text-decoration: none; color: white;" to="/categoria">Categoria</router-link>
          </v-list-tile-title>
        </v-list-tile-content>
      </v-list-tile>
    </v-list>
  </v-navigation-drawer>
</template>

<script>
export default {
  name: 'SideBar',
  data(){
    return{
      drawer: true
    }
  }
}
</script>

<style scoped>

</style>