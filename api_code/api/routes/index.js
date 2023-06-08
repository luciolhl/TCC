const bodyParser = require('body-parser')
const tags = require('./tagsRoute')
const categorias = require('./categoriasRoutes')
const turmas = require('./turmasRoutes')

module.exports = app => {
  app.use(bodyParser.json(), tags, categorias)
}
